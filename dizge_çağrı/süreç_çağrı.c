// ⵢ TAMGA:46CD | Batuhan ALGÜL | Apache-2.0 | süreç_çağrı.c
// BENGÜ = (TAMGA_145 × ONGAN_125) mod 65536

#include "dizge_çağrı.h"
#include "../süreç/süreç.h"
#include "../süreç/koşturucu.h"
#include "../bellek/sanal_bellek.h"
#include "../bellek/çerçeve_yönetici.h"
#include "../bellek/sayfalama.h"
#include "../ortak/dizge.h"

static int64_t dç_çatal(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;

	surec_t *ana = surec_geçerli_al();
	if (!ana)
		return -11;

	surec_t *çocuk = yigin_ayır(sizeof(surec_t));
	if (!çocuk)
		return -12;

	bellek_kopyala(çocuk, ana, sizeof(surec_t));

	çocuk->skn      = surec_oluştur(
		ana->ad,
		ana->öncelik,
		BOŞ)->skn;
	çocuk->ana_skn  = ana->skn;
	çocuk->durum    = SUREC_HAZIR;
	çocuk->tik_sayacı = 0;

	çocuk->adres_uzayı = sanal_uzay_oluştur();
	if (!çocuk->adres_uzayı) {
		yigin_serbest(çocuk);
		return -12;
	}

	sanal_bölge_t *bölge = ana->adres_uzayı->bölgeler;
	while (bölge) {
		boyut_t sayfa_sayısı =
			(bölge->boyut + SAYFA_BOYUTU - 1) /
			SAYFA_BOYUTU;
		for (boyut_t i = 0; i < sayfa_sayısı; i++) {
			uintptr_t sa =
				bölge->taban + i * SAYFA_BOYUTU;
			uintptr_t ga =
				sanal_gerçek_adres_bul(
					ana->adres_uzayı, sa);
			if (!ga) {
				bölge = bölge->sonraki;
				continue;
			}
			uintptr_t yeni_çerçeve = çerçeve_ayır();
			if (!yeni_çerçeve)
				return -12;
			bellek_kopyala(
				(void *)yeni_çerçeve,
				(void *)ga,
				SAYFA_BOYUTU);
			sanal_eşle(çocuk->adres_uzayı,
				sa, yeni_çerçeve,
				SAYFA_BOYUTU,
				bölge->bayraklar);
		}
		bölge = bölge->sonraki;
	}

	çocuk->yazmaçlar.rax = 0;
	ana->yazmaçlar.rax   = (uint64_t)çocuk->skn;

	kosturucu_surec_ekle(çocuk);
	return (int64_t)çocuk->skn;
}

static int64_t dç_çalıştır(const dç_çerçeve_t *çerçeve)
{
	uintptr_t giriş_noktası = (uintptr_t)çerçeve->arg0;
	const char *ad          = (const char *)çerçeve->arg1;

	if (!giriş_noktası)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli)
		return -11;

	if (ad)
		dizge_kopyala(geçerli->ad, ad, SUREC_AD_AZAMI);

	sanal_uzay_yok_et(geçerli->adres_uzayı);
	geçerli->adres_uzayı = sanal_uzay_oluştur();
	if (!geçerli->adres_uzayı)
		return -12;

	bellek_doldur(&geçerli->yazmaçlar, 0,
		sizeof(yazmaç_durumu_t));

	geçerli->yazmaçlar.rip    = giriş_noktası;
	geçerli->yazmaçlar.rsp    = geçerli->çekirdek_yığın_üst;
	geçerli->yazmaçlar.cs     = 0x08;
	geçerli->yazmaçlar.ss     = 0x10;
	geçerli->yazmaçlar.rflags = 0x202;

	sanal_uzay_etkinleştir(geçerli->adres_uzayı);
	return 0;
}

static int64_t dç_bekli(const dç_çerçeve_t *çerçeve)
{
	uint32_t hedef_skn  = (uint32_t)çerçeve->arg0;
	int32_t *durum_ptr  = (int32_t *)çerçeve->arg1;

	surec_t *hedef = surec_skn_bul(hedef_skn);
	if (!hedef)
		return -10;

	surec_t *geçerli = surec_geçerli_al();
	while (hedef->durum != SUREC_ZOMBİ &&
	       hedef->durum != SUREC_ÖLMÜŞ) {
		surec_uyut(geçerli, 10);
		kosturucu_gönüllü_bırak();
	}

	if (durum_ptr)
		*durum_ptr = hedef->çıkış_kodu;

	int32_t çıkış_kodu = hedef->çıkış_kodu;
	surec_yok_et(hedef);
	return (int64_t)çıkış_kodu;
}

static int64_t dç_öncelik_ayır(const dç_çerçeve_t *çerçeve)
{
	uint32_t	skn      = (uint32_t)çerçeve->arg0;
	uint8_t		öncelik  = (uint8_t)çerçeve->arg1;

	if (öncelik >= KOSTURUCU_ÖNCELIK_SAYISI)
		return -22;

	surec_t *hedef = skn
		? surec_skn_bul(skn)
		: surec_geçerli_al();

	if (!hedef)
		return -10;

	kosturucu_surec_çıkar(hedef);
	hedef->öncelik = (surec_öncelik_t)öncelik;
	kosturucu_surec_ekle(hedef);
	return 0;
}

static int64_t dç_öncelik_al(const dç_çerçeve_t *çerçeve)
{
	uint32_t skn = (uint32_t)çerçeve->arg0;
	surec_t *hedef = skn
		? surec_skn_bul(skn)
		: surec_geçerli_al();
	if (!hedef)
		return -10;
	return (int64_t)hedef->öncelik;
}

void süreç_çağrılarını_kaydet(void)
{
	dizge_çağrı_işleyici_kaydet(DÇ_ÇATAL,         dç_çatal);
	dizge_çağrı_işleyici_kaydet(DÇ_ÇALIŞTIIR,     dç_çalıştır);
	dizge_çağrı_işleyici_kaydet(DÇ_BEKLİ,         dç_bekli);
	dizge_çağrı_işleyici_kaydet(DÇ_ÖNCELIK_AYİR,  dç_öncelik_ayır);
	dizge_çağrı_işleyici_kaydet(DÇ_ÖNCELIK_AL,    dç_öncelik_al);
}
