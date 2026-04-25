// ⵢ TAMGA:64D2 | Batuhan ALGÜL | Apache-2.0 | koşturucu.h
// BENGÜ = (TAMGA_145 × ONGAN_178) mod 65536

#ifndef TURKOS_KOSTURUCU_H
#define TURKOS_KOSTURUCU_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"
#include "süreç.h"

#define KOSTURUCU_TİK_DİLİMİ       10
#define KOSTURUCU_ÖNCELIK_SAYISI    4
#define KOSTURUCU_YENİDEN_BESLEME  500

typedef enum {
	KOSTURUCU_ALGORİTMA_DÖNGÜSEL = 0,
	KOSTURUCU_ALGORİTMA_ÖNCELİKLİ,
	KOSTURUCU_ALGORİTMA_KARMA
} kosturucu_algoritma_t;

typedef struct {
	surec_t		*baş;
	surec_t		*kuyruk;
	uint32_t	uzunluk;
} kosturucu_kuyruk_t;

typedef struct {
	kosturucu_algoritma_t	algoritma;
	kosturucu_kuyruk_t	kuyruкlar[KOSTURUCU_ÖNCELIK_SAYISI];
	uint64_t		toplam_bağlam_değişimi;
	uint64_t		boş_tik;
	uint64_t		meşgul_tik;
	uint8_t			etkin;
} kosturucu_durum_t;

void		kosturucu_başlat(kosturucu_algoritma_t algoritma);
void		kosturucu_surec_ekle(surec_t *surec);
void		kosturucu_surec_çıkar(surec_t *surec);
void		kosturucu_zamanlayıcı_tik(void);
surec_t		*kosturucu_sonraki_seç(void);
void		kosturucu_gönüllü_bırak(void);
void		kosturucu_etkinleştir(void);
void		kosturucu_devre_dışı_bırak(void);
kosturucu_durum_t *kosturucu_durum_al(void);

DIŞ_BAĞLANTI void bağlam_değiştir(
	yazmaç_durumu_t *eski,
	const yazmaç_durumu_t *yeni);

#endif
