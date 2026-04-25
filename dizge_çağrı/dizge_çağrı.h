// ⵢ TAMGA:3C0A | Batuhan ALGÜL | Apache-2.0 | dizge_çağrı.h
// BENGÜ = (TAMGA_145 × ONGAN_106) mod 65536

#ifndef TURKOS_DIZGE_CAGRI_H
#define TURKOS_DIZGE_CAGRI_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define DÇ_ADET                 64

#define DÇ_ÇIKIŞ               0
#define DÇ_YAZI                 1
#define DÇ_OKU                  2
#define DÇ_AÇ                   3
#define DÇ_KAPAT                4
#define DÇ_STAT                 5
#define DÇ_BELLEKAYİR          6
#define DÇ_BELLEKSERBESTLE     7
#define DÇ_YENİDENBOYUTLA      8
#define DÇ_ÇATAL                9
#define DÇ_ÇALIŞTIIR           10
#define DÇ_BEKLİ               11
#define DÇ_SKN_AL              12
#define DÇ_ANA_SKN_AL          13
#define DÇ_UYUT                14
#define DÇ_ZAMAN_AL            15
#define DÇ_BÖLÜT_AYİR         16
#define DÇ_BÖLÜT_SERBESTİ     17
#define DÇ_MESAJ_GÖNDER        18
#define DÇ_MESAJ_AL            19
#define DÇ_KANAL_AÇ            20
#define DÇ_KANAL_KAPAT         21
#define DÇ_DİZİN_AÇ           22
#define DÇ_DİZİN_OKU          23
#define DÇ_DİZİN_OLUŞTUR      24
#define DÇ_DİZİN_SİL          25
#define DÇ_DOSYA_SİL          26
#define DÇ_DOSYA_TAŞİ         27
#define DÇ_BAĞLANTI_OLUŞTUR   28
#define DÇ_SEMBOL_BAĞ         29
#define DÇ_SEMBOL_OKU         30
#define DÇ_KILANIIM_AL        31
#define DÇ_SİNYAL             32
#define DÇ_SİNYAL_MASKELİ    33
#define DÇ_ÖNCELIK_AYİR      34
#define DÇ_ÖNCELIK_AL        35
#define DÇ_SOKET_AÇ          36
#define DÇ_SOKET_BAĞ         37
#define DÇ_SOKET_DİNLE       38
#define DÇ_SOKET_KABUL       39
#define DÇ_SOKET_GÖNDER      40
#define DÇ_SOKET_AL          41
#define DÇ_ÇEKIRDEK_BİLGİ   42
#define DÇ_HATA_AL           43

typedef struct {
	uint64_t	numara;
	uint64_t	arg0;
	uint64_t	arg1;
	uint64_t	arg2;
	uint64_t	arg3;
	uint64_t	arg4;
} dç_çerçeve_t;

typedef int64_t (*dç_işleyici_t)(const dç_çerçeve_t *çerçeve);

void		dizge_çağrı_altsistemi_başlat(void);
void		dizge_çağrı_işleyici_kaydet(
			uint64_t numara,
			dç_işleyici_t işleyici);
int64_t		dizge_çağrı_gönder(dç_çerçeve_t *çerçeve);

DIŞ_BAĞLANTI int64_t	son_hata;

#endif
