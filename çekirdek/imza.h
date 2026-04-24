// ⵢ TAMGA:20DA | Batuhan ALGÜL | Apache-2.0 | imza.h
// BENGÜ = (TAMGA_145 × ONGAN_58) mod 65536

#ifndef TURKOS_IMZA_H
#define TURKOS_IMZA_H

#include "../ortak/türler.h"

#define TAMGA_DEĞERİ            145
#define BENGÜ_ÇEKİRDEK          0x2CBF

#define TURKOS_SÜRÜM_ANA        0
#define TURKOS_SÜRÜM_ALT        2
#define TURKOS_SÜRÜM_YAMA       0

#define TURKOS_AD               "TURKOS"
#define TURKOS_TAM_AD           "TURKOS CEKIRDEK"
#define TURKOS_SURUM_DIZGE      "v0.2.0"
#define TURKOS_ALT_BASLIK       "[ANADOLU OZU]"
#define TURKOS_YAZAR            "Batuhan ALGUL"

#define IMZA_CERCEVE_GEN        50
#define IMZA_CERCEVE_KOSE       '+'
#define IMZA_CERCEVE_YATAY      '-'
#define IMZA_CERCEVE_DIKEY      '|'

typedef struct {
	uint16_t	tamga;
	uint16_t	bengü;
	uint8_t		geçerli;
} imza_dogrulama_t;

void			imza_göster(void);
imza_dogrulama_t	imza_dogrula(void);

#endif
