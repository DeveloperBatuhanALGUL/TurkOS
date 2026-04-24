// ⵢ TAMGA:6F04 | Batuhan ALGÜL | Apache-2.0 | küresel_bölüt.h
// BENGÜ = (TAMGA_145 × ONGAN_196) mod 65536

#ifndef DONANIM_KBT_KURESEL_BOLUT_H
#define DONANIM_KBT_KURESEL_BOLUT_H

#include "ortak/türler.h"
#include "ortak/bağlantı.h"

struct kbt_tanımlayıcı {
	uint16_t sınır_alt;
	uint16_t taban_alt;
	uint8_t  taban_orta;
	uint8_t  erişim;
	uint8_t  sınır_üst : 4;
	uint8_t  ayarlar   : 4;
	uint8_t  taban_üst;
} DÜZENLİ;

struct kbt_gösterici {
	uint16_t sınır;
	uint64_t taban;
} DÜZENLİ;

#define KBT_BOŞ_SEÇİCİ 0x00
#define KBT_KOD_SEÇİCİ 0x08
#define KBT_VERİ_SEÇİCİ 0x10

#define KBT_ERİŞİM_KOD  0x9A
#define KBT_ERİŞİM_VERİ 0x92
#define KBT_GRANÜL_4K   0xCF

extern struct kbt_tanımlayıcı kbt_tablo[];
extern struct kbt_gösterici   kbt_gösterge;

void kbt_giriş_ayarla(uint8_t indeks, uint32_t taban, uint32_t sınır, uint8_t erişim);
void kbt_ilkle(void);

#endif
