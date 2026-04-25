// ⵢ TAMGA:4092 | Batuhan ALGÜL | Apache-2.0 | sanal_bellek.c
// BENGÜ = (TAMGA_145 × ONGAN_114) mod 65536

#include "sanal_bellek.h"
#include "yığın_yönetici.h"
#include "../ortak/dizge.h"

static sanal_adres_uzayı_t	çekirdek_uzayı;
static uint8_t			çekirdek_uzayı_hazır = YANLIŞ;

static sanal_bölge_t *bölge_oluştur(
	uintptr_t taban, boyut_t boyut, uint64_t bayraklar)
{
	sanal_bölge_t *bölge = yigin_ayır(sizeof(sanal_bölge_t));
	if (!bölge)
		return BOŞ;
	bölge->taban   = taban;
	bölge->boyut   = boyut;
	bölge->bayraklar = bayraklar;
	bölge->sonraki = BOŞ;
	return bölge;
}

static void bölge_listesine_ekle(
	sanal_adres_uzayı_t *uzay, sanal_bölge_t *yeni)
{
	yeni->sonraki  = uzay->bölgeler;
	uzay->bölgeler = yeni;
}

static sanal_bölge_t *bölge_bul(
	const sanal_adres_uzayı_t *uzay, uintptr_t adres)
{
	sanal_bölge_t *geçerli = uzay->bölgeler;
	while (geçerli) {
		if (adres >= geçerli->taban &&
		    adres < geçerli->taban + geçerli->boyut)
			return geçerli;
		geçerli = geçerli->sonraki;
	}
	return BOŞ;
}

sanal_adres_uzayı_t *sanal_uzay_oluştur(void)
{
	sanal_adres_uzayı_t *uzay =
		yigin_ayır(sizeof(sanal_adres_uzayı_t));
	if (!uzay)
		return BOŞ;

	uintptr_t pml4_gerçek = çerçeve_ayır();
	if (!pml4_gerçek) {
		yigin_serbest(uzay);
		return BOŞ;
	}

	uzay->pml4        = (sayfa_tablosu_t *)pml4_gerçek;
	uzay->pml4_gerçek = pml4_gerçek;
	uzay->bölgeler    = BOŞ;
	uzay->kullanıcı_sonu = SANAL_KULLANICI_TABAN;

	bellek_doldur(uzay->pml4, 0, sizeof(sayfa_tablosu_t));

	if (çekirdek_uzayı_hazır) {
		uint16_t çekirdek_pml4_d =
			SAYFA_PML4_DİZİN(SANAL_ÇEKIRDEK_TABAN);
		uzay->pml4->girdiler[çekirdek_pml4_d] =
			çekirdek_uzayı.pml4->girdiler[çekirdek_pml4_d];
	}

	return uzay;
}

void sanal_uzay_yok_et(sanal_adres_uzayı_t *uzay)
{
	if (!uzay)
		return;

	sanal_bölge_t *geçerli = uzay->bölgeler;
	while (geçerli) {
		sanal_bölge_t *sonraki = geçerli->sonraki;
		sanal_eşlemeyi_kaldır(uzay,
			geçerli->taban, geçerli->boyut);
		yigin_serbest(geçerli);
		geçerli = sonraki;
	}

	çerçeve_serbest(uzay->pml4_gerçek);
	yigin_serbest(uzay);
}

void sanal_uzay_etkinleştir(const sanal_adres_uzayı_t *uzay)
{
	__asm__ volatile (
		"mov %0, %%cr3"
		: : "r"(uzay->pml4_gerçek) : "memory"
	);
}

int32_t sanal_eşle(
	sanal_adres_uzayı_t *uzay,
	uintptr_t sanal_adres,
	uintptr_t gerçek_adres,
	boyut_t boyut,
	uint64_t bayraklar)
{
	if (!uzay || !boyut)
		return -1;

	boyut_t sayfa_sayısı = (boyut + SAYFA_BOYUTU - 1) / SAYFA_BOYUTU;

	for (boyut_t i = 0; i < sayfa_sayısı; i++) {
		uintptr_t sa = sanal_adres  + i * SAYFA_BOYUTU;
		uintptr_t ga = gerçek_adres + i * SAYFA_BOYUTU;
		sayfa_eşle(sa, ga, bayraklar);
	}

	sanal_bölge_t *bölge = bölge_oluştur(sanal_adres, boyut, bayraklar);
	if (bölge)
		bölge_listesine_ekle(uzay, bölge);

	return 0;
}

void sanal_eşlemeyi_kaldır(
	sanal_adres_uzayı_t *uzay,
	uintptr_t sanal_adres,
	boyut_t boyut)
{
	if (!uzay || !boyut)
		return;

	boyut_t sayfa_sayısı = (boyut + SAYFA_BOYUTU - 1) / SAYFA_BOYUTU;
	for (boyut_t i = 0; i < sayfa_sayısı; i++)
		sayfa_eşlemeyi_kaldır(sanal_adres + i * SAYFA_BOYUTU);
}

uintptr_t sanal_gerçek_adres_bul(
	const sanal_adres_uzayı_t *uzay,
	uintptr_t sanal_adres)
{
	(void)uzay;
	return sanal_adres_çevir(sanal_adres);
}

int32_t sanal_sayfa_hatası_işle(
	sanal_adres_uzayı_t *uzay,
	uintptr_t hata_adresi,
	uint64_t hata_kodu)
{
	(void)hata_kodu;

	sanal_bölge_t *bölge = bölge_bul(uzay, hata_adresi);
	if (!bölge)
		return -1;

	uintptr_t sayfa_taban =
		hata_adresi & ~(SAYFA_BOYUTU - 1);
	uintptr_t yeni_çerçeve = çerçeve_ayır();
	if (!yeni_çerçeve)
		return -1;

	bellek_doldur((void *)yeni_çerçeve, 0, SAYFA_BOYUTU);
	sayfa_eşle(sayfa_taban, yeni_çerçeve, bölge->bayraklar);
	return 0;
}

void çekirdek_sanal_uzay_başlat(void)
{
	uintptr_t pml4_gerçek = çerçeve_ayır();

	çekirdek_uzayı.pml4        = (sayfa_tablosu_t *)pml4_gerçek;
	çekirdek_uzayı.pml4_gerçek = pml4_gerçek;
	çekirdek_uzayı.bölgeler    = BOŞ;
	çekirdek_uzayı.kullanıcı_sonu = SANAL_KULLANICI_TABAN;

	bellek_doldur(çekirdek_uzayı.pml4, 0, sizeof(sayfa_tablosu_t));

	sanal_eşle(&çekirdek_uzayı,
		0x0, 0x0, 0x400000, SANAL_EŞLE_ÇEKIRDEK);

	sanal_uzay_etkinleştir(&çekirdek_uzayı);
	çekirdek_uzayı_hazır = DOĞRU;
}

sanal_adres_uzayı_t *çekirdek_sanal_uzay_al(void)
{
	return &çekirdek_uzayı;
}
