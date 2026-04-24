// ⵢ TAMGA:6F95 | Batuhan ALGÜL | Apache-2.0 | yığın_yönetici.c
// BENGÜ = (TAMGA_145 × ONGAN_197) mod 65536

#include "yığın_yönetici.h"
#include "../ortak/dizge.h"

static yigin_blok_t	*yigin_baş = BOŞ;
static uintptr_t	yigin_başlangıç_adres = 0;
static boyut_t		yigin_toplam_boyut = 0;

static boyut_t hizala(boyut_t boyut)
{
	return (boyut + YIGIN_HİZALAMA - 1) & ~(YIGIN_HİZALAMA - 1);
}

static void blok_böl(yigin_blok_t *blok, boyut_t boyut)
{
	boyut_t kalan = blok->boyut - boyut - sizeof(yigin_blok_t);
	if (kalan < sizeof(yigin_blok_t) + YIGIN_HİZALAMA)
		return;

	yigin_blok_t *yeni = (yigin_blok_t *)
		((uintptr_t)blok + sizeof(yigin_blok_t) + boyut);

	yeni->sihir   = YIGIN_BLOK_SİHİR;
	yeni->boyut   = kalan;
	yeni->dolu    = YANLIŞ;
	yeni->sonraki = blok->sonraki;
	yeni->önceki  = blok;

	if (blok->sonraki)
		blok->sonraki->önceki = yeni;

	blok->sonraki = yeni;
	blok->boyut   = boyut;
}

static void blokları_birleştir(yigin_blok_t *blok)
{
	while (blok->sonraki &&
	       blok->sonraki->sihir == YIGIN_BLOK_SİHİR &&
	       !blok->sonraki->dolu) {
		yigin_blok_t *birleşen = blok->sonraki;
		blok->boyut  += sizeof(yigin_blok_t) + birleşen->boyut;
		blok->sonraki = birleşen->sonraki;
		if (birleşen->sonraki)
			birleşen->sonraki->önceki = blok;
		birleşen->sihir = YIGIN_BLOK_SİHİR_BOŞ;
	}
}

void yigin_başlat(uintptr_t başlangıç, boyut_t boyut)
{
	yigin_başlangıç_adres = başlangıç;
	yigin_toplam_boyut    = boyut;

	yigin_baş = (yigin_blok_t *)başlangıç;
	yigin_baş->sihir   = YIGIN_BLOK_SİHİR;
	yigin_baş->boyut   = boyut - sizeof(yigin_blok_t);
	yigin_baş->dolu    = YANLIŞ;
	yigin_baş->sonraki = BOŞ;
	yigin_baş->önceki  = BOŞ;
}

void *yigin_ayır(boyut_t boyut)
{
	if (!boyut || !yigin_baş)
		return BOŞ;

	boyut_t hizalı_boyut = hizala(boyut);
	yigin_blok_t *geçerli = yigin_baş;

	while (geçerli) {
		if (!geçerli->dolu &&
		    geçerli->boyut >= hizalı_boyut &&
		    geçerli->sihir == YIGIN_BLOK_SİHİR) {
			blok_böl(geçerli, hizalı_boyut);
			geçerli->dolu = DOĞRU;
			return (void *)((uintptr_t)geçerli +
				sizeof(yigin_blok_t));
		}
		geçerli = geçerli->sonraki;
	}
	return BOŞ;
}

void yigin_serbest(void *gösterge)
{
	if (!gösterge)
		return;

	yigin_blok_t *blok = (yigin_blok_t *)
		((uintptr_t)gösterge - sizeof(yigin_blok_t));

	if (blok->sihir != YIGIN_BLOK_SİHİR)
		return;

	blok->dolu = YANLIŞ;

	if (blok->önceki && !blok->önceki->dolu)
		blok = blok->önceki;

	blokları_birleştir(blok);
}

void *yigin_yeniden_boyutla(void *gösterge, boyut_t yeni_boyut)
{
	if (!gösterge)
		return yigin_ayır(yeni_boyut);

	yigin_blok_t *blok = (yigin_blok_t *)
		((uintptr_t)gösterge - sizeof(yigin_blok_t));

	if (blok->sihir != YIGIN_BLOK_SİHİR)
		return BOŞ;

	if (blok->boyut >= hizala(yeni_boyut))
		return gösterge;

	void *yeni = yigin_ayır(yeni_boyut);
	if (!yeni)
		return BOŞ;

	bellek_kopyala(yeni, gösterge, blok->boyut);
	yigin_serbest(gösterge);
	return yeni;
}

boyut_t yigin_boş_alan(void)
{
	boyut_t toplam = 0;
	yigin_blok_t *geçerli = yigin_baş;
	while (geçerli) {
		if (!geçerli->dolu)
			toplam += geçerli->boyut;
		geçerli = geçerli->sonraki;
	}
	return toplam;
}
