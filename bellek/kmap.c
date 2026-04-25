// ⵢ TAMGA:1CE3 | Batuhan ALGÜL | Apache-2.0 | kmap.c
// BENGÜ = (TAMGA_145 × ONGAN_51) mod 65536

#include "kmap.h"
#include "sanal_bellek.h"
#include "../ortak/dizge.h"

static kmap_bölge_t bölge_tablosu[KMAP_BÖLGE_SAYISI] = {
	{
		KMAP_TÜR_DÜŞÜK_BELLEK,
		KMAP_DÜŞÜK_BELLEK_TABAN,
		KMAP_DÜŞÜK_BELLEK_TABAN,
		KMAP_DÜŞÜK_BELLEK_SONU - KMAP_DÜŞÜK_BELLEK_TABAN,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR,
		"dusuk-bellek"
	},
	{
		KMAP_TÜR_VGA,
		KMAP_VGA_TAMPON,
		KMAP_VGA_TAMPON,
		KMAP_VGA_BOYUTU,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR | SAYFA_ÖNBELLEK_YOK,
		"vga-tamponu"
	},
	{
		KMAP_TÜR_ÇEKİRDEK,
		KMAP_ÇEKİRDEK_TABAN,
		KMAP_ÇEKİRDEK_TABAN,
		KMAP_ÇEKİRDEK_AZAMI_BOYUT,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR,
		"cekirdek-kodu"
	},
	{
		KMAP_TÜR_BITMAP,
		KMAP_CERCEVE_BİTMAP,
		KMAP_CERCEVE_BİTMAP,
		KMAP_CERCEVE_BİTMAP_BOYUT,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR,
		"cerceve-bitmap"
	},
	{
		KMAP_TÜR_YIGIN,
		KMAP_YIGIN_TABAN,
		KMAP_YIGIN_TABAN,
		KMAP_YIGIN_BOYUTU,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR,
		"yigin-alani"
	},
	{
		KMAP_TÜR_YIGIN_ÖBEK,
		KMAP_YIGIN_ÖBEK_TABAN,
		KMAP_YIGIN_ÖBEK_TABAN,
		KMAP_YIGIN_ÖBEK_BOYUTU,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR,
		"yigin-obek"
	},
	{
		KMAP_TÜR_MMIO,
		KMAP_MMIO_TABAN,
		KMAP_MMIO_TABAN,
		KMAP_MMIO_SONU - KMAP_MMIO_TABAN,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR | SAYFA_ÖNBELLEK_YOK,
		"mmio-alani"
	},
	{
		KMAP_TÜR_KULLANICI,
		SANAL_KULLANICI_TABAN,
		SANAL_KULLANICI_TABAN,
		SANAL_KULLANICI_SONU - SANAL_KULLANICI_TABAN,
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR | SAYFA_KULLANICI,
		"kullanici-alani"
	}
};

static uint8_t kmap_hazır = YANLIŞ;

void kmap_başlat(void)
{
	sanal_adres_uzayı_t *uzay = çekirdek_sanal_uzay_al();

	for (uint8_t i = 0; i < KMAP_BÖLGE_SAYISI; i++) {
		kmap_bölge_t *b = &bölge_tablosu[i];
		if (b->tür == KMAP_TÜR_KULLANICI)
			continue;
		sanal_eşle(uzay,
			b->sanal_taban,
			b->gerçek_taban,
			b->boyut,
			b->bayraklar);
	}

	kmap_hazır = DOĞRU;
}

const kmap_bölge_t *kmap_bölge_al(kmap_tür_t tür)
{
	for (uint8_t i = 0; i < KMAP_BÖLGE_SAYISI; i++) {
		if (bölge_tablosu[i].tür == tür)
			return &bölge_tablosu[i];
	}
	return BOŞ;
}

const kmap_bölge_t *kmap_tüm_bölgeler(uint8_t *adet)
{
	if (adet)
		*adet = KMAP_BÖLGE_SAYISI;
	return bölge_tablosu;
}

uint8_t kmap_adres_geçerli_mi(uintptr_t adres)
{
	for (uint8_t i = 0; i < KMAP_BÖLGE_SAYISI; i++) {
		const kmap_bölge_t *b = &bölge_tablosu[i];
		if (adres >= b->sanal_taban &&
		    adres <  b->sanal_taban + b->boyut)
			return DOĞRU;
	}
	return YANLIŞ;
}

kmap_tür_t kmap_adres_türü(uintptr_t adres)
{
	for (uint8_t i = 0; i < KMAP_BÖLGE_SAYISI; i++) {
		const kmap_bölge_t *b = &bölge_tablosu[i];
		if (adres >= b->sanal_taban &&
		    adres <  b->sanal_taban + b->boyut)
			return b->tür;
	}
	return KMAP_TÜR_KULLANICI;
}
