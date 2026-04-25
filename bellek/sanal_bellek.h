// ⵢ TAMGA:4092 | Batuhan ALGÜL | Apache-2.0 | sanal_bellek.h
// BENGÜ = (TAMGA_145 × ONGAN_114) mod 65536

#ifndef TURKOS_SANAL_BELLEK_H
#define TURKOS_SANAL_BELLEK_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"
#include "sayfalama.h"
#include "çerçeve_yönetici.h"

#define SANAL_ÇEKIRDEK_TABAN    0xFFFFFFFF80000000ULL
#define SANAL_ÇEKIRDEK_SONU     0xFFFFFFFFFFFFFFFFULL
#define SANAL_KULLANICI_TABAN   0x0000000000400000ULL
#define SANAL_KULLANICI_SONU    0x00007FFFFFFFFFFFULL
#define SANAL_YIGIN_TABAN       0x00007FFF00000000ULL
#define SANAL_YIGIN_BOYUTU      0x0000000000200000ULL

#define SANAL_EŞLE_ÇEKIRDEK     (SAYFA_VAR_BİT | SAYFA_YAZILABİLİR)
#define SANAL_EŞLE_KULLANICI    (SAYFA_VAR_BİT | SAYFA_YAZILABİLİR | \
				 SAYFA_KULLANICI)
#define SANAL_EŞLE_SALT_OKUNUR  (SAYFA_VAR_BİT)
#define SANAL_EŞLE_YÜRÜTÜLEBİLİR (SAYFA_VAR_BİT | SAYFA_YAZILABİLİR)

typedef struct sanal_bölge {
	uintptr_t		taban;
	boyut_t			boyut;
	uint64_t		bayraklar;
	struct sanal_bölge	*sonraki;
} sanal_bölge_t;

typedef struct {
	sayfa_tablosu_t		*pml4;
	uintptr_t		pml4_gerçek;
	sanal_bölge_t		*bölgeler;
	uintptr_t		kullanıcı_sonu;
} sanal_adres_uzayı_t;

sanal_adres_uzayı_t	*sanal_uzay_oluştur(void);
void			sanal_uzay_yok_et(sanal_adres_uzayı_t *uzay);
void			sanal_uzay_etkinleştir(
				const sanal_adres_uzayı_t *uzay);
int32_t			sanal_eşle(
				sanal_adres_uzayı_t *uzay,
				uintptr_t sanal_adres,
				uintptr_t gerçek_adres,
				boyut_t boyut,
				uint64_t bayraklar);
void			sanal_eşlemeyi_kaldır(
				sanal_adres_uzayı_t *uzay,
				uintptr_t sanal_adres,
				boyut_t boyut);
uintptr_t		sanal_gerçek_adres_bul(
				const sanal_adres_uzayı_t *uzay,
				uintptr_t sanal_adres);
int32_t			sanal_sayfa_hatası_işle(
				sanal_adres_uzayı_t *uzay,
				uintptr_t hata_adresi,
				uint64_t hata_kodu);

void			çekirdek_sanal_uzay_başlat(void);
sanal_adres_uzayı_t	*çekirdek_sanal_uzay_al(void);

#endif
