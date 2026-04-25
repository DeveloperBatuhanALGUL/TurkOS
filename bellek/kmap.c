// ⵢ TAMGA:1CE3 | Batuhan ALGÜL | Apache-2.0 | kmap.c
// BENGÜ = (TAMGA_145 × ONGAN_51) mod 65536

#include "kmap.h"
#include "sanal_bellek.h"
#include "../sürücüler/ekran/görüntü.h"
#include "../ortak/dizge.h"

static kmap_bölge_t bölgeler[KMAP_BÖLGE_SAYISI] = {
	{
		KMAP_VGA_TABAN,
		KMAP_VGA_BOYUTU,
		KMAP_TÜR_DONANIM,
		"VGA-TAMPON",
		DOĞRU,
		YANLIŞ
	},
	{
		KMAP_BIOS_TABAN,
		KMAP_BIOS_BOYUTU,
		KMAP_TÜR_AYRILI,
		"BIOS-ROM",
		YANLIŞ,
		YANLIŞ
	},
	{
		KMAP_ÇEKIRDEK_TABAN,
		KMAP_ÇEKIRDEK_AZAMI - KMAP_ÇEKIRDEK_TABAN,
		KMAP_TÜR_ÇEKIRDEK,
		"CEKIRDEK-KOD",
		DOĞRU,
		DOĞRU
	},
	{
		KMAP_BITMAP_TABAN,
		KMAP_BITMAP_AZAMI,
		KMAP_TÜR_BITMAP,
		"CERCEVE-BITMAP",
		DOĞRU,
		YANLIŞ
	},
	{
		KMAP_YIGIN_TABAN,
		KMAP_YIGIN_BOYUTU,
		KMAP_TÜR_YIGIN,
		"YIGIN-YONETICISI",
		DOĞRU,
		YANLIŞ
	},
	{
		KMAP_ÇERÇEVE_TABAN,
		KMAP_ÇERÇEVE_AZAMI,
		KMAP_TÜR_ÇERÇEVE,
		"SERBEST-CERCEVELER",
		DOĞRU,
		YANLIŞ
	},
	{ 0, 0, KMAP_TÜR_BOŞLUK, "BOS", YANLIŞ, YANLIŞ },
	{ 0, 0, KMAP_TÜR_BOŞLUK, "BOS", YANLIŞ, YANLIŞ },
};

static uint8_t	bölge_sayısı = 6;

static void bölgeyi_eşle(const kmap_bölge_t *bölge)
{
	uint64_t bayraklar = SANAL_EŞLE_ÇEKIRDEK;
	if (!bölge->yazılabilir)
		bayraklar = SANAL_EŞLE_SALT_OKUNUR;

	sanal_eşle(
		çekirdek_sanal_uzay_al(),
		bölge->taban,
		bölge->taban,
		bölge->boyut,
		bayraklar);
}

void kmap_başlat(void)
{
	for (uint8_t i = 0; i < bölge_sayısı; i++)
		bölgeyi_eşle(&bölgeler[i]);
}

const kmap_bölge_t *kmap_bölge_bul(uintptr_t adres)
{
	for (uint8_t i = 0; i < bölge_sayısı; i++) {
		if (adres >= bölgeler[i].taban &&
		    adres < bölgeler[i].taban + bölgeler[i].boyut)
			return &bölgeler[i];
	}
	return BOŞ;
}

const kmap_bölge_t *kmap_tüm_bölgeler(uint8_t *adet)
{
	if (adet)
		*adet = bölge_sayısı;
	return bölgeler;
}

uint8_t kmap_adres_geçerli_mi(uintptr_t adres)
{
	return kmap_bölge_bul(adres) != BOŞ;
}

void kmap_bölge_yazdır(void)
{
	görüntü_renk_ayarla(
		GEA_RENK(GEA_RENK_AÇIK_GRİ, GEA_RENK_SİYAH));
	görüntü_dizge_yaz("[KMAP] Bellek bolgeleri:\n");

	for (uint8_t i = 0; i < bölge_sayısı; i++) {
		char tampon[17];

		görüntü_renk_ayarla(
			GEA_RENK(GEA_RENK_AÇIK_MAVİ, GEA_RENK_SİYAH));
		görüntü_dizge_yaz("  ");
		görüntü_dizge_yaz(bölgeler[i].ad);
		görüntü_dizge_yaz(": ");

		görüntü_renk_ayarla(
			GEA_RENK(GEA_RENK_BEYAZ, GEA_RENK_SİYAH));
		görüntü_dizge_yaz("0x");
		onaltılı_dizgeye(bölgeler[i].taban, tampon,
			sizeof(tampon));
		görüntü_dizge_yaz(tampon);
		görüntü_dizge_yaz(" - 0x");
		onaltılı_dizgeye(
			bölgeler[i].taban + bölgeler[i].boyut,
			tampon, sizeof(tampon));
		görüntü_dizge_yaz(tampon);
		görüntü_karakter_yaz('\n');
	}
}
