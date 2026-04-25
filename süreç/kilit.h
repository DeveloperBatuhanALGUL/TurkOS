// ⵢ TAMGA:2B9D | Batuhan ALGÜL | Apache-2.0 | kilit.h
// BENGÜ = (TAMGA_145 × ONGAN_77) mod 65536

#ifndef TURKOS_KİLİT_H
#define TURKOS_KİLİT_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define KİLİT_AÇIK      0
#define KİLİT_KAPALI    1

#define KİLİT_AZAMI_BEKLEYİCİ  32

typedef struct {
	volatile uint32_t	değer;
} döngü_kilit_t;

typedef struct {
	volatile uint32_t	kilitli;
	volatile uint32_t	sahip_skn;
	volatile uint32_t	derinlik;
} öz_yinelemeli_kilit_t;

typedef struct {
	volatile uint32_t	kilitli;
	volatile uint32_t	sahip_skn;
	uint32_t		bekleyiciler[KİLİT_AZAMI_BEKLEYİCİ];
	volatile uint8_t	bekleyici_sayısı;
} uyku_kilidi_t;

typedef struct {
	volatile uint32_t	sayaç;
	uint32_t		azami;
} sayaç_kilidi_t;

SATIR_İÇİ void döngü_kilit_başlat(döngü_kilit_t *kilit)
{
	kilit->değer = KİLİT_AÇIK;
}

SATIR_İÇİ void döngü_kilit_al(döngü_kilit_t *kilit)
{
	while (__sync_lock_test_and_set(&kilit->değer, KİLİT_KAPALI))
		__asm__ volatile ("pause");
}

SATIR_İÇİ void döngü_kilit_bırak(döngü_kilit_t *kilit)
{
	__sync_lock_release(&kilit->değer);
}

SATIR_İÇİ uint8_t döngü_kilit_dene(döngü_kilit_t *kilit)
{
	return !__sync_lock_test_and_set(&kilit->değer, KİLİT_KAPALI);
}

void öz_yinelemeli_kilit_başlat(öz_yinelemeli_kilit_t *kilit);
void öz_yinelemeli_kilit_al(öz_yinelemeli_kilit_t *kilit);
void öz_yinelemeli_kilit_bırak(öz_yinelemeli_kilit_t *kilit);

void uyku_kilidi_başlat(uyku_kilidi_t *kilit);
void uyku_kilidi_al(uyku_kilidi_t *kilit);
void uyku_kilidi_bırak(uyku_kilidi_t *kilit);

void sayaç_kilidi_başlat(sayaç_kilidi_t *kilit, uint32_t azami);
void sayaç_kilidi_al(sayaç_kilidi_t *kilit);
void sayaç_kilidi_bırak(sayaç_kilidi_t *kilit);
uint32_t sayaç_kilidi_değer(const sayaç_kilidi_t *kilit);

SATIR_İÇİ void kesmesiz_bölge_gir(uint64_t *bayrak)
{
	__asm__ volatile (
		"pushfq\n\t"
		"pop %0\n\t"
		"cli\n\t"
		: "=r"(*bayrak) : : "memory"
	);
}

SATIR_İÇİ void kesmesiz_bölge_çık(uint64_t bayrak)
{
	if (bayrak & 0x200)
		__asm__ volatile ("sti" : : : "memory");
}

#endif
