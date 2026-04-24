// ⵢ TAMGA:21FC | Batuhan ALGÜL | Apache-2.0 | dizge.c
// BENGÜ = (TAMGA_145 × ONGAN_60) mod 65536

#include "dizge.h"

boyut_t dizge_uzunluk(const char *dizge)
{
	boyut_t uzunluk = 0;
	while (dizge[uzunluk] != '\0')
		uzunluk++;
	return uzunluk;
}

int32_t dizge_karşılaştır(const char *a, const char *b)
{
	while (*a && (*a == *b)) {
		a++;
		b++;
	}
	return (uint8_t)*a - (uint8_t)*b;
}

char *dizge_kopyala(char *hedef, const char *kaynak, boyut_t sınır)
{
	boyut_t i = 0;
	while (i + 1 < sınır && kaynak[i] != '\0') {
		hedef[i] = kaynak[i];
		i++;
	}
	if (sınır > 0)
		hedef[i] = '\0';
	return hedef;
}

char *dizge_birleştir(char *hedef, const char *kaynak, boyut_t sınır)
{
	boyut_t h = dizge_uzunluk(hedef);
	boyut_t i = 0;
	while (h + i + 1 < sınır && kaynak[i] != '\0') {
		hedef[h + i] = kaynak[i];
		i++;
	}
	hedef[h + i] = '\0';
	return hedef;
}

const char *dizge_bul(const char *dizge, char karakter)
{
	while (*dizge != '\0') {
		if (*dizge == karakter)
			return dizge;
		dizge++;
	}
	return BOŞ;
}

void *bellek_doldur(void *hedef, uint8_t değer, boyut_t boyut)
{
	uint8_t *gösterge = (uint8_t *)hedef;
	for (boyut_t i = 0; i < boyut; i++)
		gösterge[i] = değer;
	return hedef;
}

void *bellek_kopyala(void *hedef, const void *kaynak, boyut_t boyut)
{
	uint8_t *h = (uint8_t *)hedef;
	const uint8_t *k = (const uint8_t *)kaynak;
	for (boyut_t i = 0; i < boyut; i++)
		h[i] = k[i];
	return hedef;
}

int32_t bellek_karşılaştır(const void *a, const void *b, boyut_t boyut)
{
	const uint8_t *ga = (const uint8_t *)a;
	const uint8_t *gb = (const uint8_t *)b;
	for (boyut_t i = 0; i < boyut; i++) {
		if (ga[i] != gb[i])
			return (int32_t)ga[i] - (int32_t)gb[i];
	}
	return 0;
}

void tam_sayı_dizgeye(int64_t değer, char *tampon, boyut_t sınır)
{
	char geçici[21];
	uint8_t eksi = 0;
	boyut_t i = 0;
	if (değer < 0) {
		eksi = 1;
		değer = -değer;
	}
	if (değer == 0) {
		geçici[i++] = '0';
	} else {
		while (değer > 0) {
			geçici[i++] = '0' + (değer % 10);
			değer /= 10;
		}
	}
	if (eksi)
		geçici[i++] = '-';
	boyut_t j = 0;
	while (i > 0 && j + 1 < sınır)
		tampon[j++] = geçici[--i];
	tampon[j] = '\0';
}

void onaltılı_dizgeye(uint64_t değer, char *tampon, boyut_t sınır)
{
	const char basamak[] = "0123456789ABCDEF";
	char geçici[17];
	boyut_t i = 0;
	if (değer == 0) {
		geçici[i++] = '0';
	} else {
		while (değer > 0) {
			geçici[i++] = basamak[değer & 0xF];
			değer >>= 4;
		}
	}
	boyut_t j = 0;
	while (i > 0 && j + 1 < sınır)
		tampon[j++] = geçici[--i];
	tampon[j] = '\0';
}
