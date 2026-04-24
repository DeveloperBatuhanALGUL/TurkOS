// ⵢ TAMGA:4B55 | Batuhan ALGÜL | Apache-2.0 | zamanlayıcı.c
// BENGÜ = (TAMGA_145 × ONGAN_133) mod 65536

#include "zamanlayıcı.h"
#include "../../donanım/kkt/kesme_kapisi.h"

static volatile uint64_t tik_sayacı = 0;
static uint32_t	tik_hz = ZÜB_HEDEF_FREKANS;

static void porta_yaz(uint16_t porta, uint8_t değer)
{
	__asm__ volatile ("outb %0, %1" : : "a"(değer), "Nd"(porta));
}

void zamanlayıcı_işleyici(void)
{
	tik_sayacı++;
}

uint64_t zamanlayıcı_tik_sayısı(void)
{
	return tik_sayacı;
}

void zamanlayıcı_bekle(uint64_t milisaniye)
{
	uint64_t hedef = tik_sayacı + (milisaniye * tik_hz / 1000);
	while (tik_sayacı < hedef)
		__asm__ volatile ("pause");
}

void zamanlayıcı_başlat(uint32_t frekans)
{
	tik_hz = frekans;
	uint32_t bölücü = ZÜB_TEMEL_FREKANS / frekans;

	porta_yaz(ZÜB_KOMUT_PORTU,  ZÜB_AYAR_BAYRAGI);
	porta_yaz(ZÜB_KANAL0_PORTU, (uint8_t)(bölücü & 0xFF));
	porta_yaz(ZÜB_KANAL0_PORTU, (uint8_t)((bölücü >> 8) & 0xFF));

	kkt_işleyici_kaydet(0x20, zamanlayıcı_işleyici);
}
