// ⵢ TAMGA:4B55 | Batuhan ALGÜL | Apache-2.0 | zamanlayıcı.h
// BENGÜ = (TAMGA_145 × ONGAN_133) mod 65536

#ifndef TURKOS_ZAMANLAYICI_H
#define TURKOS_ZAMANLAYICI_H

#include "../../ortak/türler.h"
#include "../../ortak/bağlantı.h"

#define ZÜB_TEMEL_FREKANS       1193182
#define ZÜB_HEDEF_FREKANS       100
#define ZÜB_BÖLÜCÜ             (ZÜB_TEMEL_FREKANS / ZÜB_HEDEF_FREKANS)

#define ZÜB_KOMUT_PORTU         0x43
#define ZÜB_KANAL0_PORTU        0x40

#define ZÜB_İKİLİ_KİP          0x00
#define ZÜB_KİP_3              0x06
#define ZÜB_ERIŞIM_LOBYTE       0x30
#define ZÜB_KANAL_0             0x00

#define ZÜB_AYAR_BAYRAGI        \
	(ZÜB_KANAL_0 | ZÜB_ERIŞIM_LOBYTE | ZÜB_KİP_3 | ZÜB_İKİLİ_KİP)

void		zamanlayıcı_başlat(uint32_t frekans);
void		zamanlayıcı_işleyici(void);
uint64_t	zamanlayıcı_tik_sayısı(void);
void		zamanlayıcı_bekle(uint64_t milisaniye);

#endif
