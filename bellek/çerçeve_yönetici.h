// ⵢ TAMGA:6E73 | Batuhan ALGÜL | Apache-2.0 | çerçeve_yönetici.h
// BENGÜ = (TAMGA_145 × ONGAN_195) mod 65536

#ifndef TURKOS_CERCEVE_YONETICI_H
#define TURKOS_CERCEVE_YONETICI_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define SAYFA_BOYUTU            0x1000UL
#define CERCEVE_HARITA_TABAN    0x500000UL
#define CERCEVE_AZAMI_BELLEK    0x100000000UL

#define MULTIBOOT2_SİHİR        0x36D76289UL

#define MB2_EtiKET_BELLEK_HARİTASI  6

#define MB2_BÖLGE_KULLANILABİLİR   1
#define MB2_BÖLGE_AYRILMIŞ         2
#define MB2_BÖLGE_ACPI_GERİ_AL     3
#define MB2_BÖLGE_NVS              4
#define MB2_BÖLGE_HATALII          5

typedef struct {
	uint32_t	toplam_etiket_boyutu;
	uint32_t	ayrılmış;
} PAKETLENMIŞ mb2_başlık_t;

typedef struct {
	uint32_t	tür;
	uint32_t	boyut;
} PAKETLENMIŞ mb2_etiket_t;

typedef struct {
	uint32_t	tür;
	uint32_t	boyut;
	uint32_t	girdi_boyutu;
	uint32_t	sürüm;
} PAKETLENMIŞ mb2_bellek_haritası_başlık_t;

typedef struct {
	uint64_t	taban_adres;
	uint64_t	uzunluk;
	uint32_t	tür;
	uint32_t	ayrılmış;
} PAKETLENMIŞ mb2_bellek_bölgesi_t;

typedef struct {
	uint64_t	toplam_çerçeve;
	uint64_t	boş_çerçeve;
	uint64_t	kullanılan_çerçeve;
	uintptr_t	bitmap_taban;
	boyut_t		bitmap_boyut;
} çerçeve_istatistik_t;

void		çerçeve_yönetici_başlat(uintptr_t mb2_bilgi_adres);
uintptr_t	çerçeve_ayır(void);
void		çerçeve_serbest(uintptr_t çerçeve_adres);
void		çerçeve_bölgeyi_işaretle(
			uintptr_t taban, boyut_t boyut, uint8_t dolu);
çerçeve_istatistik_t	çerçeve_istatistik_al(void);
uint8_t		çerçeve_kullanımda_mı(uintptr_t adres);

#endif
