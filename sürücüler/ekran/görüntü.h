// ⵢ TAMGA:4FDD | Batuhan ALGÜL | Apache-2.0 | görüntü.h
// BENGÜ = (TAMGA_145 × ONGAN_141) mod 65536

#ifndef TURKOS_GORUNTU_H
#define TURKOS_GORUNTU_H

#include "../../ortak/türler.h"

#define GEA_TAMPON_ADRES        0xB8000
#define GEA_SÜTUN_SAYISI        80
#define GEA_SATIR_SAYISI        25
#define GEA_HÜCRE_SAYISI        (GEA_SÜTUN_SAYISI * GEA_SATIR_SAYISI)

#define GEA_RENK_SİYAH          0x0
#define GEA_RENK_MAVİ           0x1
#define GEA_RENK_YEŞİL          0x2
#define GEA_RENK_CAMGÖBEĞI      0x3
#define GEA_RENK_KIRMIZI        0x4
#define GEA_RENK_MOR            0x5
#define GEA_RENK_KAHVERENGİ     0x6
#define GEA_RENK_AÇIK_GRİ      0x7
#define GEA_RENK_KOYU_GRİ      0x8
#define GEA_RENK_AÇIK_MAVİ    0x9
#define GEA_RENK_AÇIK_YEŞİL   0xA
#define GEA_RENK_AÇIK_CAMG    0xB
#define GEA_RENK_AÇIK_KIRMIZI 0xC
#define GEA_RENK_AÇIK_MOR     0xD
#define GEA_RENK_SARI           0xE
#define GEA_RENK_BEYAZ          0xF

#define GEA_RENK(ön, arka)      (((arka) << 4) | (ön))
#define GEA_VARSAYILAN_RENK     GEA_RENK(GEA_RENK_AÇIK_GRİ, GEA_RENK_SİYAH)

void	görüntü_başlat(void);
void	görüntü_temizle(void);
void	görüntü_karakter_yaz(char karakter);
void	görüntü_dizge_yaz(const char *dizge);
void	görüntü_sayı_yaz(uint64_t değer);
void	görüntü_onaltılı_yaz(uint64_t değer);
void	görüntü_renk_ayarla(uint8_t renk);
void	görüntü_konum_ayarla(uint8_t sütun, uint8_t satır);
void	görüntü_yeni_satır(void);

#endif
