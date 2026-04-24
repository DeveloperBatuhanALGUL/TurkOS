// ⵢ TAMGA:5221 | Batuhan ALGÜL | Apache-2.0 | kesme_kapisi.h
// BENGÜ = (TAMGA_145 × ONGAN_145) mod 65536

#ifndef TURKOS_KESME_KAPISI_H
#define TURKOS_KESME_KAPISI_H

#include "../../ortak/türler.h"
#include "../../ortak/bağlantı.h"

#define KKT_KAPISI_SAYISI       256

#define KKT_VAR_BIT             (1 << 7)
#define KKT_HALKA_0             0
#define KKT_HALKA_3             (3 << 5)
#define KKT_KESİM_KAPISI_64     0xE
#define KKT_TUZAK_KAPISI_64     0xF

#define KDY_ANA_KOMUT           0x20
#define KDY_ANA_VERİ            0x21
#define KDY_İKİNCİL_KOMUT       0xA0
#define KDY_İKİNCİL_VERİ        0xA1
#define KDY_BAŞLAT              0x11
#define KDY_8086_KİP            0x01
#define KDY_BİTİŞ               0x20

#define KOD_BÖLÜT_SEÇİCİ       0x08

typedef struct {
	uint16_t	kaydırım_alt;
	uint16_t	bölüt_seçici;
	uint8_t		yığın_tablosu;
	uint8_t		tür_bayraklar;
	uint16_t	kaydırım_orta;
	uint32_t	kaydırım_üst;
	uint32_t	ayrılmış;
} PAKETLENMIŞ kkt_girdisi_t;

typedef struct {
	uint16_t	sınır;
	uint64_t	taban;
} PAKETLENMIŞ kkt_gösterge_t;

typedef struct {
	uint64_t	rip;
	uint64_t	cs;
	uint64_t	rflags;
	uint64_t	rsp;
	uint64_t	ss;
} PAKETLENMIŞ kesme_çerçeve_t;

void kkt_başlat(void);
void kkt_işleyici_kaydet(uint8_t vektör, void (*işleyici)(void));

DIŞ_BAĞLANTI void kesme_işleyici_genel(
	kesme_çerçeve_t *çerçeve, uint64_t vektör);

#endif
