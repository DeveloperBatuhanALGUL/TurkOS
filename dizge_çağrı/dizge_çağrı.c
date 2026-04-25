// ⵢ TAMGA:3C0A | Batuhan ALGÜL | Apache-2.0 | dizge_çağrı.c
// BENGÜ = (TAMGA_145 × ONGAN_106) mod 65536

#include "dizge_çağrı.h"
#include "../süreç/süreç.h"
#include "../ortak/dizge.h"

#define MSR_EFER        0xC0000080
#define MSR_STAR        0xC0000081
#define MSR_LSTAR       0xC0000082
#define MSR_FMASK       0xC0000084
#define EFER_SCE        (1ULL << 0)
#define RFLAGS_IF       (1ULL << 9)
#define RFLAGS_DF       (1ULL << 10)

int64_t son_hata = 0;

static dç_işleyici_t	işleyici_tablosu[DÇ_ADET];

static void msr_yaz(uint32_t msr, uint64_t değer)
{
	uint32_t düşük = değer & 0xFFFFFFFF;
	uint32_t yüksek = değer >> 32;
	__asm__ volatile (
		"wrmsr"
		: : "c"(msr), "a"(düşük), "d"(yüksek)
	);
}

static uint64_t msr_oku(uint32_t msr)
{
	uint32_t düşük, yüksek;
	__asm__ volatile (
		"rdmsr"
		: "=a"(düşük), "=d"(yüksek)
		: "c"(msr)
	);
	return ((uint64_t)yüksek << 32) | düşük;
}

static int64_t dç_geçersiz(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	son_hata = -38;
	return -38;
}

static int64_t dç_hata_al(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	return son_hata;
}

static int64_t dç_skn_al(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	surec_t *geçerli = surec_geçerli_al();
	return geçerli ? (int64_t)geçerli->skn : -1;
}

static int64_t dç_ana_skn_al(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	surec_t *geçerli = surec_geçerli_al();
	return geçerli ? (int64_t)geçerli->ana_skn : -1;
}

static int64_t dç_çıkış(const dç_çerçeve_t *çerçeve)
{
	surec_çıkış((int32_t)çerçeve->arg0);
	return 0;
}

static int64_t dç_uyut(const dç_çerçeve_t *çerçeve)
{
	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli)
		return -1;
	surec_uyut(geçerli, (uint64_t)çerçeve->arg0);
	return 0;
}

static int64_t dç_çekirdek_bilgi(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	return 0x0200;
}

DIŞ_BAĞLANTI void syscall_giriş_noktası(void);

int64_t dizge_çağrı_gönder(dç_çerçeve_t *çerçeve)
{
	if (!çerçeve || çerçeve->numara >= DÇ_ADET) {
		son_hata = -38;
		return -38;
	}

	dç_işleyici_t işleyici = işleyici_tablosu[çerçeve->numara];
	if (!işleyici) {
		son_hata = -38;
		return -38;
	}

	int64_t sonuç = işleyici(çerçeve);
	if (sonuç < 0)
		son_hata = sonuç;

	return sonuç;
}

void dizge_çağrı_işleyici_kaydet(
	uint64_t numara, dç_işleyici_t işleyici)
{
	if (numara < DÇ_ADET)
		işleyici_tablosu[numara] = işleyici;
}

void dizge_çağrı_altsistemi_başlat(void)
{
	for (uint64_t i = 0; i < DÇ_ADET; i++)
		işleyici_tablosu[i] = dç_geçersiz;

	dizge_çağrı_işleyici_kaydet(DÇ_ÇIKIŞ,       dç_çıkış);
	dizge_çağrı_işleyici_kaydet(DÇ_SKN_AL,      dç_skn_al);
	dizge_çağrı_işleyici_kaydet(DÇ_ANA_SKN_AL,  dç_ana_skn_al);
	dizge_çağrı_işleyici_kaydet(DÇ_UYUT,         dç_uyut);
	dizge_çağrı_işleyici_kaydet(DÇ_HATA_AL,     dç_hata_al);
	dizge_çağrı_işleyici_kaydet(DÇ_ÇEKİRDEK_BİLGİ, dç_çekirdek_bilgi);

	uint64_t efer = msr_oku(MSR_EFER);
	msr_yaz(MSR_EFER, efer | EFER_SCE);

	msr_yaz(MSR_STAR,
		((uint64_t)0x0008 << 32) |
		((uint64_t)0x0018 << 48));

	msr_yaz(MSR_LSTAR, (uint64_t)syscall_giriş_noktası);
	msr_yaz(MSR_FMASK, RFLAGS_IF | RFLAGS_DF);
}
