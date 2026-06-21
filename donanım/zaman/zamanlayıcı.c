#include "zamanlayıcı.h"
#include "../../ortak/bağlantı.h"
#include "../kkt/kesme_kapisi.h"

static volatile unsigned int tik_sayaci = 0;

static void zamanlayici_isleyici(void)
{
    tik_sayaci++;
}

unsigned int zamanlayici_tik_sayisi(void)
{
    return tik_sayaci;
}

void zamanlayici_baslat(unsigned int frekans)
{
    unsigned int bolen = 1193180 / frekans;

    port_byte_yaz(0x43, 0x36);
    port_byte_yaz(0x40, bolen & 0xFF);
    port_byte_yaz(0x40, (bolen >> 8) & 0xFF);

    kesme_yoneticisi_kaydet(32, zamanlayici_isleyici);
}
