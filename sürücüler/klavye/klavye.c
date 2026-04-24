// ⵢ TAMGA:338B | Batuhan ALGÜL | Apache-2.0 | klavye.c
// BENGÜ = (TAMGA_145 × ONGAN_91) mod 65536

#include "klavye.h"
#include "../ekran/görüntü.h"
#include "../../donanım/kkt/kesme_kapisi.h"

static tua_durum_t tua;

static const char tarama_tablosu_normal[128] = {
	0,   27,  '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\n', 0,  'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0,  '\\','z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0,   '*',
	0,   ' ', 0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
};

static const char tarama_tablosu_shift[128] = {
	0,   27,  '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}', '\n', 0,  'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'"', '~', 0,  '|', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0,   '*',
	0,   ' ', 0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
};

static uint8_t portadan_oku(uint16_t porta)
{
	uint8_t değer;
	__asm__ volatile ("inb %1, %0" : "=a"(değer) : "Nd"(porta));
	return değer;
}

static void tampon_ekle(char karakter)
{
	uint8_t sonraki = (tua.yazma_konumu + 1) % TUA_TAMPON_BOYUTU;
	if (sonraki == tua.okuma_konumu)
		return;
	tua.tampon[tua.yazma_konumu] = karakter;
	tua.yazma_konumu = sonraki;
}

void klavye_işleyici(void)
{
	uint8_t tarama_kodu = portadan_oku(TUA_VERİ_PORTU);
	uint8_t bırakıldı   = tarama_kodu & TUA_BIRAKILDI_BİTİ;
	uint8_t tuş         = tarama_kodu & ~TUA_BIRAKILDI_BİTİ;

	if (tuş == TUA_ÖZEL_SOL_SHIFT || tuş == TUA_ÖZEL_SAĞ_SHIFT) {
		tua.shift_basılı = bırakıldı ? YANLIŞ : DOĞRU;
		return;
	}
	if (tuş == TUA_ÖZEL_SOL_CTRL) {
		tua.ctrl_basılı = bırakıldı ? YANLIŞ : DOĞRU;
		return;
	}
	if (tuş == TUA_ÖZEL_CAPS_LOCK && !bırakıldı) {
		tua.caps_lock = !tua.caps_lock;
		return;
	}
	if (bırakıldı || tuş >= 128)
		return;

	uint8_t büyük_harf = tua.shift_basılı ^ tua.caps_lock;
	char karakter = büyük_harf
		? tarama_tablosu_shift[tuş]
		: tarama_tablosu_normal[tuş];

	if (karakter)
		tampon_ekle(karakter);
}

uint8_t klavye_tampon_dolu_mu(void)
{
	return tua.okuma_konumu != tua.yazma_konumu;
}

char klavye_karakter_oku(void)
{
	if (!klavye_tampon_dolu_mu())
		return 0;
	char karakter = tua.tampon[tua.okuma_konumu];
	tua.okuma_konumu = (tua.okuma_konumu + 1) % TUA_TAMPON_BOYUTU;
	return karakter;
}

void klavye_başlat(void)
{
	tua.yazma_konumu = 0;
	tua.okuma_konumu = 0;
	tua.shift_basılı = YANLIŞ;
	tua.ctrl_basılı  = YANLIŞ;
	tua.caps_lock    = YANLIŞ;
	kkt_işleyici_kaydet(TUA_IRQ, klavye_işleyici);
}
