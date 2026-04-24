// ⵢ TAMGA:6E73 | Batuhan ALGÜL | Apache-2.0 | çerçeve_yönetici.c
// BENGÜ = (TAMGA_145 × ONGAN_195) mod 65536

#include "çerçeve_yönetici.h"
#include "../ortak/dizge.h"

static uint8_t		*bitmap		= (uint8_t *)CERCEVE_HARITA_TABAN;
static uint64_t		toplam_çerçeve	= 0;
static uint64_t		boş_çerçeve	= 0;
static uint64_t		bitmap_boyut	= 0;
static uintptr_t	bellek_sonu	= 0;

static void bitmap_bit_ayarla(uint64_t dizin)
{
	bitmap[dizin / 8] |= (1 << (dizin % 8));
}

static void bitmap_bit_temizle(uint64_t dizin)
{
	bitmap[dizin / 8] &= ~(1 << (dizin % 8));
}

static uint8_t bitmap_bit_oku(uint64_t dizin)
{
	return (bitmap[dizin / 8] >> (dizin % 8)) & 1;
}

static uint64_t adres_çerçeve_dizin(uintptr_t adres)
{
	return adres / SAYFA_BOYUTU;
}

void çerçeve_bölgeyi_işaretle(
	uintptr_t taban, boyut_t boyut, uint8_t dolu)
{
	uint64_t başlangıç = adres_çerçeve_dizin(taban);
	uint64_t adet      = boyut / SAYFA_BOYUTU;

	for (uint64_t i = 0; i < adet; i++) {
		if (başlangıç + i >= toplam_çerçeve)
			break;
		if (dolu) {
			if (!bitmap_bit_oku(başlangıç + i)) {
				bitmap_bit_ayarla(başlangıç + i);
				if (boş_çerçeve > 0)
					boş_çerçeve--;
			}
		} else {
			if (bitmap_bit_oku(başlangıç + i)) {
				bitmap_bit_temizle(başlangıç + i);
				boş_çerçeve++;
			}
		}
	}
}

static void mb2_bellek_haritasını_işle(uintptr_t mb2_adres)
{
	mb2_başlık_t *başlık = (mb2_başlık_t *)mb2_adres;
	uintptr_t	etiket_adres = mb2_adres + sizeof(mb2_başlık_t);
	uintptr_t	bitiş_adres  =
		mb2_adres + başlık->toplam_etiket_boyutu;

	while (etiket_adres < bitiş_adres) {
		mb2_etiket_t *etiket = (mb2_etiket_t *)etiket_adres;

		if (etiket->tür == 0)
			break;

		if (etiket->tür == MB2_EtiKET_BELLEK_HARİTASI) {
			mb2_bellek_haritası_başlık_t *hb =
				(mb2_bellek_haritası_başlık_t *)etiket;

			uintptr_t girdi_adres =
				etiket_adres +
				sizeof(mb2_bellek_haritası_başlık_t);
			uintptr_t girdi_sonu =
				etiket_adres + etiket->boyut;

			while (girdi_adres < girdi_sonu) {
				mb2_bellek_bölgesi_t *bölge =
					(mb2_bellek_bölgesi_t *)girdi_adres;

				uintptr_t bölge_sonu =
					bölge->taban_adres + bölge->uzunluk;

				if (bölge_sonu > bellek_sonu)
					bellek_sonu = bölge_sonu;

				if (bölge->tür == MB2_BÖLGE_KULLANILABİLİR)
					çerçeve_bölgeyi_işaretle(
						(uintptr_t)bölge->taban_adres,
						(boyut_t)bölge->uzunluk,
						YANLIŞ);

				girdi_adres += hb->girdi_boyutu;
			}
		}

		etiket_adres += (etiket->boyut + 7) & ~7UL;
	}
}

static void çekirdek_bölgesini_rezerve_et(void)
{
	DIŞ_BAĞLANTI uint8_t _başlangıç[];
	DIŞ_BAĞLANTI uint8_t _son[];

	uintptr_t çekirdek_başlangıç = (uintptr_t)_başlangıç;
	uintptr_t çekirdek_sonu      = (uintptr_t)_son;
	boyut_t   çekirdek_boyutu    =
		çekirdek_sonu - çekirdek_başlangıç;

	çerçeve_bölgeyi_işaretle(
		çekirdek_başlangıç, çekirdek_boyutu, DOĞRU);
}

static void bitmap_bölgesini_rezerve_et(void)
{
	çerçeve_bölgeyi_işaretle(
		CERCEVE_HARITA_TABAN, bitmap_boyut, DOĞRU);
}

static void düşük_belleği_rezerve_et(void)
{
	çerçeve_bölgeyi_işaretle(0, 0x100000, DOĞRU);
}

void çerçeve_yönetici_başlat(uintptr_t mb2_bilgi_adres)
{
	bellek_sonu    = 0;
	boş_çerçeve   = 0;

	mb2_başlık_t *başlık = (mb2_başlık_t *)mb2_bilgi_adres;
	uintptr_t	etiket_adres =
		mb2_bilgi_adres + sizeof(mb2_başlık_t);
	uintptr_t	bitiş_adres  =
		mb2_bilgi_adres + başlık->toplam_etiket_boyutu;

	while (etiket_adres < bitiş_adres) {
		mb2_etiket_t *etiket = (mb2_etiket_t *)etiket_adres;
		if (etiket->tür == 0)
			break;
		if (etiket->tür == MB2_EtiKET_BELLEK_HARİTASI) {
			mb2_bellek_haritası_başlık_t *hb =
				(mb2_bellek_haritası_başlık_t *)etiket;
			uintptr_t ga =
				etiket_adres +
				sizeof(mb2_bellek_haritası_başlık_t);
			uintptr_t gs = etiket_adres + etiket->boyut;
			while (ga < gs) {
				mb2_bellek_bölgesi_t *b =
					(mb2_bellek_bölgesi_t *)ga;
				uintptr_t son =
					b->taban_adres + b->uzunluk;
				if (son > bellek_sonu)
					bellek_sonu = son;
				ga += hb->girdi_boyutu;
			}
		}
		etiket_adres += (etiket->boyut + 7) & ~7UL;
	}

	toplam_çerçeve = bellek_sonu / SAYFA_BOYUTU;
	bitmap_boyut   = (toplam_çerçeve + 7) / 8;

	bellek_doldur(bitmap, 0xFF, bitmap_boyut);
	boş_çerçeve = 0;

	mb2_bellek_haritasını_işle(mb2_bilgi_adres);

	düşük_belleği_rezerve_et();
	çekirdek_bölgesini_rezerve_et();
	bitmap_bölgesini_rezerve_et();
}

uintptr_t çerçeve_ayır(void)
{
	for (uint64_t i = 0; i < toplam_çerçeve; i++) {
		if (!bitmap_bit_oku(i)) {
			bitmap_bit_ayarla(i);
			boş_çerçeve--;
			return i * SAYFA_BOYUTU;
		}
	}
	return 0;
}

void çerçeve_serbest(uintptr_t çerçeve_adres)
{
	uint64_t dizin = adres_çerçeve_dizin(çerçeve_adres);
	if (dizin >= toplam_çerçeve)
		return;
	if (!bitmap_bit_oku(dizin)) {
		bitmap_bit_temizle(dizin);
		boş_çerçeve++;
	}
}

uint8_t çerçeve_kullanımda_mı(uintptr_t adres)
{
	return bitmap_bit_oku(adres_çerçeve_dizin(adres));
}

çerçeve_istatistik_t çerçeve_istatistik_al(void)
{
	çerçeve_istatistik_t ist;
	ist.toplam_çerçeve   = toplam_çerçeve;
	ist.boş_çerçeve      = boş_çerçeve;
	ist.kullanılan_çerçeve = toplam_çerçeve - boş_çerçeve;
	ist.bitmap_taban     = CERCEVE_HARITA_TABAN;
	ist.bitmap_boyut     = bitmap_boyut;
	return ist;
}
