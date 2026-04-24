// ⵢ TAMGA:6F04 | Batuhan ALGÜL | Apache-2.0 | küresel_bölüt.h
// BENGÜ = (TAMGA_145 × ONGAN_196) mod 65536

#ifndef TURKOS_KURESEL_BOLUT_H
#define TURKOS_KURESEL_BOLUT_H

#include "../../ortak/türler.h"
#include "../../ortak/bağlantı.h"

#define KBT_BÖLÜT_SAYISI        5

#define HALKA_0                 0
#define HALKA_3                 3

#define KBT_VAR_BIT             (1 << 7)
#define KBT_KOD_BÖLÜT           (1 << 3)
#define KBT_VERİ_BÖLÜT           0
#define KBT_OKUNABILIR          (1 << 1)
#define KBT_YAZILABİLİR        (1 << 1)
#define KBT_UZUN_KİP            (1 << 5)
#define KBT_32BIT_KİP           (1 << 6)
#define KBT_BOYUT_BAYRAGI       (1 << 7)
#define KBT_S_BİT               (1 << 4)

typedef struct {
	uint16_t	sınır_alt;
	uint16_t	taban_alt;
	uint8_t		taban_orta;
	uint8_t		erişim;
	uint8_t		bayraklar_sınır_üst;
	uint8_t		taban_üst;
} PAKETLENMIŞ kbt_girdisi_t;

typedef struct {
	uint16_t	sınır;
	uint64_t	taban;
} PAKETLENMIŞ kbt_gösterge_t;

void kbt_başlat(void);

#endif
