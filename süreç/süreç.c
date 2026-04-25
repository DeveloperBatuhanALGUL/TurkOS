// ⵢ TAMGA:2CBF | Batuhan ALGÜL | Apache-2.0 | süreç.c
// BENGÜ = (TAMGA_145 × ONGAN_79) mod 65536

#include "süreç.h"
#include "../bellek/yığın_yönetici.h"
#include "../bellek/çerçeve_yönetici.h"
#include "../ortak/dizge.h"
#include "../donanım/zaman/zamanlayıcı.h"

static surec_t		*surec_listesi_baş = BOŞ;
static surec_t		*geçerli_surec     = BOŞ;
static uint32_t		sonraki_skn        = 1;
static uint32_t		surec_sayısı       = 0;

static uint32_t yeni_skn_al(void)
{
	return sonraki_skn++;
}

static surec_t *surec_listesine_ekle(surec_t *yeni)
{
	yeni->sonraki = surec_listesi_baş;
	yeni->önceki  = BOŞ;
	if (surec_listesi_baş)
		surec_listesi_baş->önceki = yeni;
	surec_listesi_baş = yeni;
	surec_sayısı++;
	return yeni;
}

static void surec_listesinden_çıkar(surec_t *surec)
{
	if (surec->önceki)
		surec->önceki->sonraki = surec->sonraki;
	else
		surec_listesi_baş = surec->sonraki;

	if (surec->sonraki)
		surec->sonraki->önceki = surec->önceki;

	surec_sayısı--;
}

static int32_t çekirdek_yığını_ayır(surec_t *surec)
{
	uintptr_t yığın = (uintptr_t)yigin_ayır(SUREC_YIGIN_BOYUTU);
	if (!yığın)
		return -1;
	surec->çekirdek_yığın_taban = yığın;
	surec->çekirdek_yığın_üst   = yığın + SUREC_YIGIN_BOYUTU;
	return 0;
}

static void yazmaçları_sıfırla(surec_t *surec, void (*giriş)(void))
{
	bellek_doldur(&surec->yazmaçlar, 0, sizeof(yazmaç_durumu_t));
	surec->yazmaçlar.rip    = (uint64_t)giriş;
	surec->yazmaçlar.rsp    = surec->çekirdek_yığın_üst;
	surec->yazmaçlar.cs     = 0x08;
	surec->yazmaçlar.ss     = 0x10;
	surec->yazmaçlar.rflags = 0x202;
}

surec_t *surec_oluştur(
	const char *ad,
	surec_öncelik_t öncelik,
	void (*giriş)(void))
{
	if (surec_sayısı >= SUREC_AZAMI_SAYISI)
		return BOŞ;

	surec_t *surec = yigin_ayır(sizeof(surec_t));
	if (!surec)
		return BOŞ;

	bellek_doldur(surec, 0, sizeof(surec_t));

	surec->skn      = yeni_skn_al();
	surec->ana_skn  = geçerli_surec
			? geçerli_surec->skn : 0;
	surec->durum    = SUREC_HAZIR;
	surec->öncelik  = öncelik;
	surec->tik_sayacı = 0;
	surec->çıkış_kodu = 0;

	dizge_kopyala(surec->ad, ad, SUREC_AD_AZAMI);

	surec->adres_uzayı = sanal_uzay_oluştur();
	if (!surec->adres_uzayı) {
		yigin_serbest(surec);
		return BOŞ;
	}

	if (çekirdek_yığını_ayır(surec) < 0) {
		sanal_uzay_yok_et(surec->adres_uzayı);
		yigin_serbest(surec);
		return BOŞ;
	}

	yazmaçları_sıfırla(surec, giriş);
	surec_listesine_ekle(surec);
	return surec;
}

void surec_yok_et(surec_t *surec)
{
	if (!surec)
		return;

	surec_listesinden_çıkar(surec);

	if (surec->adres_uzayı)
		sanal_uzay_yok_et(surec->adres_uzayı);

	if (surec->çekirdek_yığın_taban)
		yigin_serbest((void *)surec->çekirdek_yığın_taban);

	yigin_serbest(surec);
}

surec_t *surec_skn_bul(uint32_t skn)
{
	surec_t *geçerli = surec_listesi_baş;
	while (geçerli) {
		if (geçerli->skn == skn)
			return geçerli;
		geçerli = geçerli->sonraki;
	}
	return BOŞ;
}

surec_t *surec_geçerli_al(void)
{
	return geçerli_surec;
}

void surec_durumu_ayarla(surec_t *surec, surec_durum_t durum)
{
	if (surec)
		surec->durum = durum;
}

void surec_uyut(surec_t *surec, uint64_t milisaniye)
{
	if (!surec)
		return;
	surec->uyku_bitiş_tik =
		zamanlayıcı_tik_sayısı() + (milisaniye * 100 / 1000);
	surec->durum = SUREC_UYUYOR;
}

void surec_çıkış(int32_t kod)
{
	if (geçerli_surec) {
		geçerli_surec->çıkış_kodu = kod;
		geçerli_surec->durum      = SUREC_ZOMBİ;
	}
	while (DOĞRU)
		__asm__ volatile ("hlt");
}

static void boş_surec_görevi(void)
{
	while (DOĞRU)
		__asm__ volatile ("hlt");
}

void surec_altsistemi_başlat(void)
{
	surec_listesi_baş = BOŞ;
	geçerli_surec     = BOŞ;
	sonraki_skn       = 1;
	surec_sayısı      = 0;

	surec_t *boş = surec_oluştur(
		"bos-surec",
		SUREC_ÖNCELIK_DÜŞÜK,
		boş_surec_görevi);

	if (boş) {
		boş->skn   = 0;
		boş->durum = SUREC_ÇALIŞIYOR;
		geçerli_surec = boş;
	}
}
