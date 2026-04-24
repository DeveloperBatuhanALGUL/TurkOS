// ⵢ TAMGA:6F04 | Batuhan ALGÜL | Apache-2.0 | küresel_bölüt.c
// BENGÜ = (TAMGA_145 × ONGAN_196) mod 65536

#include "küresel_bölüt.h"

static kbt_girdisi_t	kbt_tablosu[KBT_BÖLÜT_SAYISI];
static kbt_gösterge_t	kbt_gösterge;

static void kbt_girdi_yaz(
	uint32_t	dizin,
	uint32_t	taban,
	uint32_t	sınır,
	uint8_t		erişim,
	uint8_t		bayraklar)
{
	kbt_tablosu[dizin].taban_alt    = taban & 0xFFFF;
	kbt_tablosu[dizin].taban_orta   = (taban >> 16) & 0xFF;
	kbt_tablosu[dizin].taban_üst    = (taban >> 24) & 0xFF;
	kbt_tablosu[dizin].sınır_alt    = sınır & 0xFFFF;
	kbt_tablosu[dizin].bayraklar_sınır_üst =
		((sınır >> 16) & 0x0F) | (bayraklar & 0xF0);
	kbt_tablosu[dizin].erişim       = erişim;
}

static void kbt_yükle(const kbt_gösterge_t *gösterge)
{
	__asm__ volatile (
		"lgdt %0\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"
		"mov %%ax, %%ss\n\t"
		"pushq $0x08\n\t"
		"lea 1f(%%rip), %%rax\n\t"
		"pushq %%rax\n\t"
		"lretq\n\t"
		"1:\n\t"
		:
		: "m"(*gösterge)
		: "rax", "memory"
	);
}

void kbt_başlat(void)
{
	kbt_girdi_yaz(0, 0, 0, 0, 0);

	kbt_girdi_yaz(1, 0, 0xFFFFF,
		KBT_VAR_BIT | KBT_S_BİT | KBT_KOD_BÖLÜT | KBT_OKUNABILIR,
		KBT_UZUN_KİP | KBT_BOYUT_BAYRAGI);

	kbt_girdi_yaz(2, 0, 0xFFFFF,
		KBT_VAR_BIT | KBT_S_BİT | KBT_VERİ_BÖLÜT | KBT_YAZILABİLİR,
		KBT_32BIT_KİP | KBT_BOYUT_BAYRAGI);

	kbt_girdi_yaz(3, 0, 0xFFFFF,
		KBT_VAR_BIT | KBT_S_BİT | KBT_KOD_BÖLÜT | KBT_OKUNABILIR |
		(HALKA_3 << 5),
		KBT_UZUN_KİP | KBT_BOYUT_BAYRAGI);

	kbt_girdi_yaz(4, 0, 0xFFFFF,
		KBT_VAR_BIT | KBT_S_BİT | KBT_VERİ_BÖLÜT | KBT_YAZILABİLİR |
		(HALKA_3 << 5),
		KBT_32BIT_KİP | KBT_BOYUT_BAYRAGI);

	kbt_gösterge.sınır = sizeof(kbt_tablosu) - 1;
	kbt_gösterge.taban = (uint64_t)kbt_tablosu;

	kbt_yükle(&kbt_gösterge);
}
