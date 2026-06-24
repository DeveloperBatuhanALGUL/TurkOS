#ifndef BELLEK_DENETLEYICI_H
#define BELLEK_DENETLEYICI_H

void bellek_denetleyici_baslat(void);
void bellek_denetleyici_tahsis_bildir(void *adres, unsigned int boyut);
void bellek_denetleyici_serbest_bildir(void *adres);
unsigned int bellek_denetleyici_acik_tahsis_sayisi(void);

#endif
