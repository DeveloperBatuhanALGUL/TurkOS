// ⵢ TAMGA:2D50 | Batuhan ALGÜL | Apache-2.0 | bağlantı.h
// BENGÜ = (TAMGA_145 × ONGAN_80) mod 65536

#ifndef TURKOS_BAGLANTI_H
#define TURKOS_BAGLANTI_H

#define DIŞ_BAĞLANTI		extern
#define İÇ_BAĞLANTI		static
#define SATIR_İÇİ		static inline
#define KULLANILMAYAN		__attribute__((unused))
#define PAKETLENMIŞ		__attribute__((packed))
#define HIZALANMIŞ(n)		__attribute__((aligned(n)))
#define SIFIR_GERİ_DÖNMEZ	__attribute__((noreturn))
#define ZAYIF_BAĞLANTI		__attribute__((weak))
#define BÖLÜM(s)		__attribute__((section(s)))

#define DERLEME_ZAMANI_DOĞRULA(koşul) \
	_Static_assert((koşul), #koşul)

#endif
