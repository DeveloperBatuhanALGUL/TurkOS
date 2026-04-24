// ⵢ TAMGA:0AC3 | Batuhan ALGÜL | Apache-2.0 | ana.c
// BENGÜ = (TAMGA_145 × ONGAN_19) mod 65536

#include "imza.h"
#include "../donanım/kbt/küresel_bölüt.h"
#include "../donanım/kkt/kesme_kapisi.h"
#include "../donanım/zaman/zamanlayıcı.h"
#include "../sürücüler/ekran/görüntü.h"
#include "../sürücüler/klavye/klavye.h"
#include "../bellek/sayfalama.h"
#include "../bellek/yığın_yönetici.h"
#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

static void durum_satırı_yaz(const char *etiket, const char *ileti)
{
	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_AÇIK_GRİ, GEA_RENK_SİYAH));
	görüntü_karakter_yaz('|');
	görüntü_karakter_yaz(' ');

	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_AÇIK_MAVİ, GEA_RENK_SİYAH));
	görüntü_dizge_yaz(etiket);

	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_AÇIK_GRİ, GEA_RENK_SİYAH));
	görüntü_karakter_yaz(' ');
	görüntü_dizge_yaz(ileti);
	görüntü_karakter_yaz('\n');
}

static void ayraç_yaz(void)
{
	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_BEYAZ, GEA_RENK_SİYAH));
	görüntü_karakter_yaz('+');
	for (uint8_t i = 0; i < 50; i++)
		görüntü_karakter_yaz('-');
	görüntü_karakter_yaz('+');
	görüntü_karakter_yaz('\n');
}

static void giriş_döngüsü(void)
{
	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_BEYAZ, GEA_RENK_SİYAH));
	görüntü_dizge_yaz("   SISTEM HAZIR. GIRIS BEKLENIYOR...\n");

	while (DOĞRU) {
		if (klavye_tampon_dolu_mu()) {
			char k = klavye_karakter_oku();
			görüntü_karakter_yaz(k);
		}
		__asm__ volatile ("hlt");
	}
}

SIFIR_GERİ_DÖNMEZ void çekirdek_ana(uint32_t multiboot_sihir)
{
	(void)multiboot_sihir;

	görüntü_başlat();
	imza_göster();

	kbt_başlat();
	ayraç_yaz();
	durum_satırı_yaz("[KBT ]",
		"Kuresel Bolut Tablosu yuklendi");

	kkt_başlat();
	durum_satırı_yaz("[KKT ]",
		"Kesme Kapisi Tablosu yuklendi");

	zamanlayıcı_başlat(ZÜB_HEDEF_FREKANS);
	durum_satırı_yaz("[ZUB ]",
		"Zamanlayici baslatildi (100Hz)");

	klavye_başlat();
	durum_satırı_yaz("[TUA ]",
		"Klavye denetleyicisi hazir");

	sayfalama_başlat();
	yigin_başlat(YIGIN_BAŞLANGIÇ_ADRES, YIGIN_AZAMI_BOYUT);
	durum_satırı_yaz("[GEA ]",
		"Goruntu arabirimi aktif");

	ayraç_yaz();

	giriş_döngüsü();
}
