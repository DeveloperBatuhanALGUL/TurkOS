// ⵢ TAMGA:21FC | Batuhan ALGÜL | Apache-2.0 | dizge.h
// BENGÜ = (TAMGA_145 × ONGAN_60) mod 65536

#ifndef TURKOS_DIZGE_H
#define TURKOS_DIZGE_H

#include "../ortak/türler.h"

boyut_t		dizge_uzunluk(const char *dizge);
int32_t		dizge_karşılaştır(const char *a, const char *b);
char		*dizge_kopyala(char *hedef, const char *kaynak, boyut_t sınır);
char		*dizge_birleştir(char *hedef, const char *kaynak, boyut_t sınır);
const char	*dizge_bul(const char *dizge, char karakter);
void		*bellek_doldur(void *hedef, uint8_t değer, boyut_t boyut);
void		*bellek_kopyala(void *hedef, const void *kaynak, boyut_t boyut);
int32_t		bellek_karşılaştır(const void *a, const void *b, boyut_t boyut);
void		tam_sayı_dizgeye(int64_t değer, char *tampon, boyut_t sınır);
void		onaltılı_dizgeye(uint64_t değer, char *tampon, boyut_t sınır);

#endif
