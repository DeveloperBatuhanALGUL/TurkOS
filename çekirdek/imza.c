// ⵢ TAMGA:20DA | Batuhan ALGÜL | Apache-2.0 | imza.c
// BENGÜ = (TAMGA_145 × ONGAN_58) mod 65536

#include "imza.h"
#include "../sürücüler/ekran/görüntü.h"
#include "../ortak/dizge.h"

static void satır_yaz(char kenar, const char *içerik)
{
	görüntü_karakter_yaz(kenar);
	görüntü_karakter_yaz(' ');
	görüntü_dizge_yaz(içerik);
	görüntü_karakter_yaz('\n');
}

static void ayraç_yaz(char sol, char orta, char sağ)
{
	görüntü_karakter_yaz(sol);
	for (uint8_t i = 0; i < IMZA_CERCEVE_GEN; i++)
		görüntü_karakter_yaz(orta);
	görüntü_karakter_yaz(sağ);
	görüntü_karakter_yaz('\n');
}

static void boş_satır_yaz(void)
{
	görüntü_karakter_yaz(IMZA_CERCEVE_DIKEY);
	for (uint8_t i = 0; i < IMZA_CERCEVE_GEN; i++)
		görüntü_karakter_yaz(' ');
	görüntü_karakter_yaz(IMZA_CERCEVE_DIKEY);
	görüntü_karakter_yaz('\n');
}

static void başlık_satırı_yaz(const char *sol, const char *sağ)
{
	char tampon[IMZA_CERCEVE_GEN + 1];
	boyut_t s_boy = dizge_uzunluk(sol);
	boyut_t g_boy = dizge_uzunluk(sağ);
	boyut_t toplam = s_boy + g_boy + 2;

	dizge_kopyala(tampon, sol, sizeof(tampon));

	if (toplam < IMZA_CERCEVE_GEN) {
		boyut_t dolgu = IMZA_CERCEVE_GEN - toplam;
		for (boyut_t i = 0; i < dolgu; i++)
			tampon[s_boy + i] = ' ';
		tampon[s_boy + dolgu] = '\0';
		dizge_birleştir(tampon, sağ, sizeof(tampon));
	}

	görüntü_karakter_yaz(IMZA_CERCEVE_DIKEY);
	görüntü_karakter_yaz(' ');
	görüntü_dizge_yaz(tampon);
	görüntü_karakter_yaz(' ');
	görüntü_karakter_yaz(IMZA_CERCEVE_DIKEY);
	görüntü_karakter_yaz('\n');
}

imza_dogrulama_t imza_dogrula(void)
{
	imza_dogrulama_t sonuç;
	sonuç.tamga  = TAMGA_DEĞERİ;
	sonuç.bengü  = BENGÜ_ÇEKİRDEK;
	sonuç.geçerli = (TAMGA_DEĞERİ == 145 &&
			 BENGÜ_ÇEKİRDEK == 0x2CBF) ? DOĞRU : YANLIŞ;
	return sonuç;
}

void imza_göster(void)
{
	imza_dogrulama_t doğrulama = imza_dogrula();

	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_BEYAZ, GEA_RENK_SİYAH));

	ayraç_yaz(IMZA_CERCEVE_KOSE, IMZA_CERCEVE_YATAY,
		  IMZA_CERCEVE_KOSE);

	başlık_satırı_yaz(
		"  " TURKOS_TAM_AD " " TURKOS_SURUM_DIZGE, "");
	başlık_satırı_yaz(
		"  " TURKOS_ALT_BASLIK, "");

	ayraç_yaz(IMZA_CERCEVE_KOSE, IMZA_CERCEVE_YATAY,
		  IMZA_CERCEVE_KOSE);

	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_SARI, GEA_RENK_SİYAH));
	başlık_satırı_yaz(
		"  \xec\xb5\xa2 TAMGA:2CBF  |  " TURKOS_YAZAR, "");

	if (doğrulama.geçerli) {
		görüntü_renk_ayarla(
			GEA_RENK(GEA_RENK_AÇIK_YEŞİL, GEA_RENK_SİYAH));
		başlık_satırı_yaz("  DOGRULAMA: GECERLI", "");
	} else {
		görüntü_renk_ayarla(
			GEA_RENK(GEA_RENK_AÇIK_KIRMIZI, GEA_RENK_SİYAH));
		başlık_satırı_yaz("  DOGRULAMA: HATALI", "");
	}

	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_BEYAZ, GEA_RENK_SİYAH));
	ayraç_yaz(IMZA_CERCEVE_KOSE, IMZA_CERCEVE_YATAY,
		  IMZA_CERCEVE_KOSE);
}
