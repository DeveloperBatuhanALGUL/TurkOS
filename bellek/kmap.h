// ⵢ TAMGA:1CE3 | Batuhan ALGÜL | Apache-2.0 | kmap.h
// BENGÜ = (TAMGA_145 × ONGAN_51) mod 65536

#ifndef TURKOS_KMAP_H
#define TURKOS_KMAP_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define KMAP_BÖLGE_SAYISI       16

#define KMAP_DÜŞÜKBELLEKBAŞ    0x0000000000000000ULL
#define KMAP_DÜŞÜKBELLEKBİT    0x0000000000100000ULL

#define KMAP_VGA_TAMPON         0x00000000000B8000ULL
#define KMAP_VGA_BOYUTU         0x0000000000001000ULL

#define KMAP_ÇEKİRDEKTABAN     0x0000000000100000ULL

#define KMAP_YIGIN_TABAN        0x0000000000400000ULL
#define KMAP_YIGIN_BOYUTU       0x0000000000400000ULL

#define KMAP_CERCEVE_BİTMAP     0x0000000000500000ULL
#define KMAP_CERCEVE_BOYUTU     0x0000000000100000ULL

#define KMAP_SÜRÜCÜ_TABAN       0x0000000001000000ULL
#define KMAP_SÜRÜCÜ_BOYUTU      0x0000000001000000ULL

#define KMAP_APIC_TABAN         0x00000000FEE00000ULL
#define KMAP_APIC_BOYUTU        0x0000000000001000ULL

typedef enum {
	KMAP_TÜR_KULLANILABİLİR = 0,
	KMAP_TÜR_ÇEKİRDEK,
	KMAP_TÜR_AYGIT,
	KMAP_TÜR_YIGIN,
	KMAP_TÜR_BITMAP,
	KMAP_TÜR_APIC,
	KMAP_TÜR_AYRILI,
} kmap_bölge_tür_t;

typedef struct {
	uintptr_t		taban;
	boyut_t			boyut;
	kmap_bölge_tür_t	tür;
	const char		*ad;
	uint8_t			eşlendi;
} kmap_bölge_t;

void			kmap_başlat(void);
const kmap_bölge_t	*kmap_bölge_bul(uintptr_t adres);
const kmap_bölge_t	*kmap_bölge_listesi_al(void);
uint8_t			kmap_bölge_sayısı_al(void);
void			kmap_aygıt_eşle(
				uintptr_t gerçek_adres,
				boyut_t boyut,
				const char *ad);
uint8_t			kmap_adres_geçerli_mi(uintptr_t adres);

#endif
