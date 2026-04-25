// ⵢ TAMGA:2CBF | Batuhan ALGÜL | Apache-2.0 | süreç.h
// BENGÜ = (TAMGA_145 × ONGAN_79) mod 65536

#ifndef TURKOS_SUREC_H
#define TURKOS_SUREC_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"
#include "../bellek/sanal_bellek.h"

#define SUREC_AD_AZAMI          32
#define SUREC_AZAMI_SAYISI      256
#define SUREC_AZAMI_DOSYA       16
#define SUREC_YIGIN_BOYUTU      0x4000

#define SUREC_GEÇERSİZ_SKN      ((uint32_t)-1)

typedef enum {
	SUREC_HAZIR = 0,
	SUREC_ÇALIŞIYOR,
	SUREC_BEKLIYOR,
	SUREC_UYUYOR,
	SUREC_ZOMBİ,
	SUREC_ÖLMÜŞ
} surec_durum_t;

typedef enum {
	SUREC_ÖNCELIK_DÜŞÜK    = 0,
	SUREC_ÖNCELIK_NORMAL   = 1,
	SUREC_ÖNCELIK_YÜKSEK   = 2,
	SUREC_ÖNCELIK_GERÇEK   = 3
} surec_öncelik_t;

typedef struct {
	uint64_t	rax, rbx, rcx, rdx;
	uint64_t	rsi, rdi, rbp, rsp;
	uint64_t	r8,  r9,  r10, r11;
	uint64_t	r12, r13, r14, r15;
	uint64_t	rip, rflags;
	uint64_t	cs,  ss;
} PAKETLENMIŞ yazmaç_durumu_t;

typedef struct surec {
	uint32_t		skn;
	uint32_t		ana_skn;
	char			ad[SUREC_AD_AZAMI];
	surec_durum_t		durum;
	surec_öncelik_t		öncelik;
	yazmaç_durumu_t		yazmaçlar;
	sanal_adres_uzayı_t	*adres_uzayı;
	uintptr_t		çekirdek_yığın_taban;
	uintptr_t		çekirdek_yığın_üst;
	uintptr_t		kullanıcı_yığın_taban;
	uintptr_t		kullanıcı_yığın_üst;
	uint64_t		tik_sayacı;
	uint64_t		uyku_bitiş_tik;
	int32_t			çıkış_kodu;
	void			*dosyalar[SUREC_AZAMI_DOSYA];
	struct surec		*sonraki;
	struct surec		*önceki;
} surec_t;

void		surec_altsistemi_başlat(void);
surec_t		*surec_oluştur(const char *ad,
			surec_öncelik_t öncelik,
			void (*giriş)(void));
void		surec_yok_et(surec_t *surec);
surec_t		*surec_skn_bul(uint32_t skn);
surec_t		*surec_geçerli_al(void);
void		surec_durumu_ayarla(surec_t *surec,
			surec_durum_t durum);
void		surec_uyut(surec_t *surec, uint64_t milisaniye);
void		surec_çıkış(int32_t kod) SIFIR_GERİ_DÖNMEZ;

#endif
