// ⵢ TAMGA:5221 | Batuhan ALGÜL | Apache-2.0 | kesme_kapisi.c
// BENGÜ = (TAMGA_145 × ONGAN_145) mod 65536

#include "kesme_kapisi.h"
#include "../../ortak/türler.h"

static kkt_girdisi_t	kkt_tablosu[KKT_KAPISI_SAYISI];
static kkt_gösterge_t	kkt_gösterge;

DIŞ_BAĞLANTI void *kesme_vektör_tablosu[KKT_KAPISI_SAYISI];

static void porta_yaz(uint16_t porta, uint8_t değer)
{
	__asm__ volatile ("outb %0, %1" : : "a"(değer), "Nd"(porta));
}

static uint8_t portadan_oku(uint16_t porta)
{
	uint8_t değer;
	__asm__ volatile ("inb %1, %0" : "=a"(değer) : "Nd"(porta));
	return değer;
}

static void kdy_yeniden_programla(void)
{
	uint8_t ana_maske  = portadan_oku(KDY_ANA_VERİ);
	uint8_t ikincil_maske = portadan_oku(KDY_İKİNCİL_VERİ);

	porta_yaz(KDY_ANA_KOMUT,    KDY_BAŞLAT);
	porta_yaz(KDY_İKİNCİL_KOMUT, KDY_BAŞLAT);

	porta_yaz(KDY_ANA_VERİ,     0x20);
	porta_yaz(KDY_İKİNCİL_VERİ, 0x28);

	porta_yaz(KDY_ANA_VERİ,     0x04);
	porta_yaz(KDY_İKİNCİL_VERİ, 0x02);

	porta_yaz(KDY_ANA_VERİ,     KDY_8086_KİP);
	porta_yaz(KDY_İKİNCİL_VERİ, KDY_8086_KİP);

	porta_yaz(KDY_ANA_VERİ,     ana_maske);
	porta_yaz(KDY_İKİNCİL_VERİ, ikincil_maske);
}

static void kkt_girdi_yaz(uint8_t vektör, void (*işleyici)(void))
{
	uint64_t adres = (uint64_t)işleyici;

	kkt_tablosu[vektör].kaydırım_alt    = adres & 0xFFFF;
	kkt_tablosu[vektör].kaydırım_orta   = (adres >> 16) & 0xFFFF;
	kkt_tablosu[vektör].kaydırım_üst    = (adres >> 32) & 0xFFFFFFFF;
	kkt_tablosu[vektör].bölüt_seçici   = KOD_BÖLÜT_SEÇİCİ;
	kkt_tablosu[vektör].yığın_tablosu  = 0;
	kkt_tablosu[vektör].tür_bayraklar  =
		KKT_VAR_BIT | (KKT_KESİM_KAPISI_64 & 0xF);
	kkt_tablosu[vektör].ayrılmış       = 0;
}

void kkt_işleyici_kaydet(uint8_t vektör, void (*işleyici)(void))
{
	kkt_girdi_yaz(vektör, işleyici);
}

void kesme_işleyici_genel(kesme_çerçeve_t *çerçeve, uint64_t vektör)
{
	(void)çerçeve;
	if (vektör >= 0x20 && vektör <= 0x2F) {
		if (vektör >= 0x28)
			porta_yaz(KDY_İKİNCİL_KOMUT, KDY_BİTİŞ);
		porta_yaz(KDY_ANA_KOMUT, KDY_BİTİŞ);
	}
}

void kkt_başlat(void)
{
	kdy_yeniden_programla();

	for (uint32_t i = 0; i < KKT_KAPISI_SAYISI; i++)
		kkt_girdi_yaz(i, (void (*)(void))kesme_vektör_tablosu[i]);

	kkt_gösterge.sınır = sizeof(kkt_tablosu) - 1;
	kkt_gösterge.taban = (uint64_t)kkt_tablosu;

	__asm__ volatile (
		"lidt %0\n\t"
		"sti\n\t"
		: : "m"(kkt_gösterge) : "memory"
	);
}
