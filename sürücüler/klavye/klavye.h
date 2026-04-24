// ⵢ TAMGA:338B | Batuhan ALGÜL | Apache-2.0 | klavye.h
// BENGÜ = (TAMGA_145 × ONGAN_91) mod 65536

#ifndef TURKOS_KLAVYE_H
#define TURKOS_KLAVYE_H

#include "../../ortak/türler.h"
#include "../../ortak/bağlantı.h"

#define TUA_VERİ_PORTU          0x60
#define TUA_DURUM_PORTU         0x64
#define TUA_IRQ                 0x21
#define TUA_BIRAKILDI_BİTİ     0x80
#define TUA_TAMPON_BOYUTU       256

#define TUA_ÖZEL_ESC            0x01
#define TUA_ÖZEL_GERİ_SİL      0x0E
#define TUA_ÖZEL_TAB            0x0F
#define TUA_ÖZEL_ENTER          0x1C
#define TUA_ÖZEL_SOL_SHIFT      0x2A
#define TUA_ÖZEL_SAĞ_SHIFT      0x36
#define TUA_ÖZEL_SOL_CTRL       0x1D
#define TUA_ÖZEL_CAPS_LOCK      0x3A

typedef struct {
	char		tampon[TUA_TAMPON_BOYUTU];
	uint8_t		yazma_konumu;
	uint8_t		okuma_konumu;
	uint8_t		shift_basılı;
	uint8_t		ctrl_basılı;
	uint8_t		caps_lock;
} tua_durum_t;

void	klavye_başlat(void);
void	klavye_işleyici(void);
uint8_t	klavye_tampon_dolu_mu(void);
char	klavye_karakter_oku(void);

#endif
