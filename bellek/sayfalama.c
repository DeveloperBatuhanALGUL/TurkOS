// ⵢ TAMGA:341C | Batuhan ALGÜL | Apache-2.0 | sayfalama.c
// BENGÜ = (TAMGA_145 × ONGAN_92) mod 65536

#include "sayfalama.h"
#include "../ortak/dizge.h"

static sayfa_tablosu_t	pml4	HIZALANMIŞ(SAYFA_BOYUTU);
static sayfa_tablosu_t	pdpt	HIZALANMIŞ(SAYFA_BOYUTU);
static sayfa_tablosu_t	pd	HIZALANMIŞ(SAYFA_BOYUTU);
static sayfa_tablosu_t	pt	HIZALANMIŞ(SAYFA_BOYUTU);

static uint64_t	sayfa_çerçeve_sayacı = 0x200000;

static uint64_t sayfa_çerçeve_ayır(void)
{
	uint64_t adres = sayfa_çerçeve_sayacı;
	sayfa_çerçeve_sayacı += SAYFA_BOYUTU;
	return adres;
}

static sayfa_tablosu_t *sonraki_tablo_al(
	sayfa_tablosu_t *tablo, uint16_t dizin, uint64_t bayraklar)
{
	if (!(tablo->girdiler[dizin] & SAYFA_VAR_BİT)) {
		uint64_t yeni_adres = sayfa_çerçeve_ayır();
		sayfa_tablosu_t *yeni = (sayfa_tablosu_t *)yeni_adres;
		bellek_doldur(yeni, 0, sizeof(sayfa_tablosu_t));
		tablo->girdiler[dizin] =
			(yeni_adres & SAYFA_ADRES_MASKESİ) | bayraklar;
	}
	return (sayfa_tablosu_t *)
		(tablo->girdiler[dizin] & SAYFA_ADRES_MASKESİ);
}

void sayfa_eşle(
	uint64_t sanal_adres,
	uint64_t gerçek_adres,
	uint64_t bayraklar)
{
	uint16_t pml4_d = SAYFA_PML4_DİZİN(sanal_adres);
	uint16_t pdpt_d = SAYFA_PDPT_DİZİN(sanal_adres);
	uint16_t pd_d   = SAYFA_PD_DİZİN(sanal_adres);
	uint16_t pt_d   = SAYFA_PT_DİZİN(sanal_adres);

	uint64_t tablo_bayrakları =
		SAYFA_VAR_BİT | SAYFA_YAZILABİLİR;

	sayfa_tablosu_t *pdpt_t =
		sonraki_tablo_al(&pml4, pml4_d, tablo_bayrakları);
	sayfa_tablosu_t *pd_t =
		sonraki_tablo_al(pdpt_t, pdpt_d, tablo_bayrakları);
	sayfa_tablosu_t *pt_t =
		sonraki_tablo_al(pd_t, pd_d, tablo_bayrakları);

	pt_t->girdiler[pt_d] =
		(gerçek_adres & SAYFA_ADRES_MASKESİ) | bayraklar;
}

void sayfa_eşlemeyi_kaldır(uint64_t sanal_adres)
{
	uint16_t pml4_d = SAYFA_PML4_DİZİN(sanal_adres);
	uint16_t pdpt_d = SAYFA_PDPT_DİZİN(sanal_adres);
	uint16_t pd_d   = SAYFA_PD_DİZİN(sanal_adres);
	uint16_t pt_d   = SAYFA_PT_DİZİN(sanal_adres);

	if (!(pml4.girdiler[pml4_d] & SAYFA_VAR_BİT))
		return;
	sayfa_tablosu_t *pdpt_t = (sayfa_tablosu_t *)
		(pml4.girdiler[pml4_d] & SAYFA_ADRES_MASKESİ);

	if (!(pdpt_t->girdiler[pdpt_d] & SAYFA_VAR_BİT))
		return;
	sayfa_tablosu_t *pd_t = (sayfa_tablosu_t *)
		(pdpt_t->girdiler[pdpt_d] & SAYFA_ADRES_MASKESİ);

	if (!(pd_t->girdiler[pd_d] & SAYFA_VAR_BİT))
		return;
	sayfa_tablosu_t *pt_t = (sayfa_tablosu_t *)
		(pd_t->girdiler[pd_d] & SAYFA_ADRES_MASKESİ);

	pt_t->girdiler[pt_d] = 0;
	__asm__ volatile ("invlpg (%0)" : : "r"(sanal_adres) : "memory");
}

uint64_t sanal_adres_çevir(uint64_t sanal_adres)
{
	uint16_t pml4_d = SAYFA_PML4_DİZİN(sanal_adres);
	uint16_t pdpt_d = SAYFA_PDPT_DİZİN(sanal_adres);
	uint16_t pd_d   = SAYFA_PD_DİZİN(sanal_adres);
	uint16_t pt_d   = SAYFA_PT_DİZİN(sanal_adres);

	if (!(pml4.girdiler[pml4_d] & SAYFA_VAR_BİT))
		return 0;
	sayfa_tablosu_t *pdpt_t = (sayfa_tablosu_t *)
		(pml4.girdiler[pml4_d] & SAYFA_ADRES_MASKESİ);

	if (!(pdpt_t->girdiler[pdpt_d] & SAYFA_VAR_BİT))
		return 0;
	sayfa_tablosu_t *pd_t = (sayfa_tablosu_t *)
		(pdpt_t->girdiler[pdpt_d] & SAYFA_ADRES_MASKESİ);

	if (!(pd_t->girdiler[pd_d] & SAYFA_VAR_BİT))
		return 0;
	sayfa_tablosu_t *pt_t = (sayfa_tablosu_t *)
		(pd_t->girdiler[pd_d] & SAYFA_ADRES_MASKESİ);

	if (!(pt_t->girdiler[pt_d] & SAYFA_VAR_BİT))
		return 0;

	return (pt_t->girdiler[pt_d] & SAYFA_ADRES_MASKESİ)
		| (sanal_adres & 0xFFF);
}

void sayfalama_başlat(void)
{
	bellek_doldur(&pml4, 0, sizeof(sayfa_tablosu_t));
	bellek_doldur(&pdpt, 0, sizeof(sayfa_tablosu_t));
	bellek_doldur(&pd,   0, sizeof(sayfa_tablosu_t));
	bellek_doldur(&pt,   0, sizeof(sayfa_tablosu_t));

	uint64_t bayraklar = SAYFA_VAR_BİT | SAYFA_YAZILABİLİR;
	for (uint64_t i = 0; i < 0x400000; i += SAYFA_BOYUTU)
		sayfa_eşle(i, i, bayraklar);

	__asm__ volatile (
		"mov %0, %%cr3"
		: : "r"((uint64_t)&pml4) : "memory"
	);
}
