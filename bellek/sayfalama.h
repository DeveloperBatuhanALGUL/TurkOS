// ⵢ TAMGA:341C | Batuhan ALGÜL | Apache-2.0 | sayfalama.h
// BENGÜ = (TAMGA_145 × ONGAN_92) mod 65536

#ifndef TURKOS_SAYFALAMA_H
#define TURKOS_SAYFALAMA_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define SAYFA_BOYUTU            0x1000
#define SAYFA_HİZALAMA          0x1000
#define SAYFA_GİRDİ_SAYISI      512

#define SAYFA_VAR_BİT           (1ULL << 0)
#define SAYFA_YAZILABİLİR      (1ULL << 1)
#define SAYFA_KULLANICI         (1ULL << 2)
#define SAYFA_YAZ_ÖNBELLEK      (1ULL << 3)
#define SAYFA_ÖNBELLEK_YOK     (1ULL << 4)
#define SAYFA_ERİŞİLDİ         (1ULL << 5)
#define SAYFA_KİRLİ            (1ULL << 6)
#define SAYFA_BÜYÜK            (1ULL << 7)
#define SAYFA_YÜRÜTÜLEMEz      (1ULL << 63)

#define SAYFA_ADRES_MASKESİ    0x000FFFFFFFFFF000ULL

#define SAYFA_PML4_DİZİN(adres) \
	(((adres) >> 39) & 0x1FF)
#define SAYFA_PDPT_DİZİN(adres) \
	(((adres) >> 30) & 0x1FF)
#define SAYFA_PD_DİZİN(adres)   \
	(((adres) >> 21) & 0x1FF)
#define SAYFA_PT_DİZİN(adres)   \
	(((adres) >> 12) & 0x1FF)

typedef uint64_t sayfa_girdisi_t;

typedef struct {
	sayfa_girdisi_t girdiler[SAYFA_GİRDİ_SAYISI];
} HIZALANMIŞ(SAYFA_BOYUTU) sayfa_tablosu_t;

void	sayfalama_başlat(void);
void	sayfa_eşle(uint64_t sanal_adres, uint64_t gerçek_adres,
		uint64_t bayraklar);
void	sayfa_eşlemeyi_kaldır(uint64_t sanal_adres);
uint64_t sanal_adres_çevir(uint64_t sanal_adres);

DIŞ_BAĞLANTI uint64_t çekirdek_son;

#endif
