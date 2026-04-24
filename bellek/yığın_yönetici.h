// ⵢ TAMGA:6F95 | Batuhan ALGÜL | Apache-2.0 | yığın_yönetici.h
// BENGÜ = (TAMGA_145 × ONGAN_197) mod 65536

#ifndef TURKOS_YIGIN_YONETICI_H
#define TURKOS_YIGIN_YONETICI_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define YIGIN_BAŞLANGIÇ_ADRES   0x400000
#define YIGIN_AZAMI_BOYUT       0x400000
#define YIGIN_HİZALAMA          16

#define YIGIN_BLOK_SİHİR        0xBABA1453UL
#define YIGIN_BLOK_SİHİR_BOŞ   0xDEADBABAUL

typedef struct yigin_blok {
	uint32_t		sihir;
	boyut_t			boyut;
	uint8_t			dolu;
	struct yigin_blok	*sonraki;
	struct yigin_blok	*önceki;
} PAKETLENMIŞ yigin_blok_t;

void	yigin_başlat(uintptr_t başlangıç, boyut_t boyut);
void	*yigin_ayır(boyut_t boyut);
void	yigin_serbest(void *gösterge);
void	*yigin_yeniden_boyutla(void *gösterge, boyut_t yeni_boyut);
boyut_t	yigin_boş_alan(void);

#endif
