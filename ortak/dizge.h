// ⵢ TAMGA:21FC | Batuhan ALGÜL | Apache-2.0 | dizge.h
// BENGÜ = (TAMGA_145 × ONGAN_60) mod 65536

#ifndef ORTAK_DIZGE_H
#define ORTAK_DIZGE_H

#include "türler.h"

bayt dizge_uzunluk(const bayt *katar);
bayt *dizge_kopyala(bayt *hedef, const bayt *kaynak, bayt sınır);
doğruluk dizge_karsilastir(const bayt *birinci, const bayt *ikinci, bayt uzunluk);
bayt *dizge_doldur(bayt *bölge, bayt değer, bayt adet);

#endif
