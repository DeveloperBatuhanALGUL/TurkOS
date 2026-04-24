// ⵢ TAMGA:21FC | Batuhan ALGÜL | Apache-2.0 | dizge.c
// BENGÜ = (TAMGA_145 × ONGAN_60) mod 65536

#include "dizge.h"
#include "bağlantı.h"

bayt dizge_uzunluk(const bayt *katar)
{
	bayt uzunluk = 0;
	while (katar[uzunluk] != '\0')
		uzunluk++;
	return uzunluk;
}

bayt *dizge_kopyala(bayt *hedef, const bayt *kaynak, bayt sınır)
{
	bayt *başlangıç = hedef;
	while (sınır > 0 && *kaynak != '\0') {
		*hedef++ = *kaynak++;
		sınır--;
	}
	*hedef = '\0';
	return başlangıç;
}

doğruluk dizge_karsilastir(const bayt *birinci, const bayt *ikinci, bayt uzunluk)
{
	while (uzunluk > 0) {
		if (*birinci != *ikinci)
			return YANLIŞ;
		if (*birinci == '\0')
			return DOĞRU;
		birinci++;
		ikinci++;
		uzunluk--;
	}
	return DOĞRU;
}

bayt *dizge_doldur(bayt *bölge, bayt değer, bayt adet)
{
	bayt *başlangıç = bölge;
	while (adet > 0) {
		*bölge++ = değer;
		adet--;
	}
	return başlangıç;
}
