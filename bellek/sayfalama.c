#include "sayfalama.h"

#define GIRDI_SAYISI 1024
#define SAYFA_BOYUTU 4096
#define ESLENEN_MB 16

static unsigned int sayfa_dizini[GIRDI_SAYISI] __attribute__((aligned(4096)));
static unsigned int sayfa_tablolari[ESLENEN_MB][GIRDI_SAYISI] __attribute__((aligned(4096)));

extern void sayfalama_etkinlestir(unsigned int *dizin_adresi);

void sayfalama_baslat(void)
{
    for (int t = 0; t < ESLENEN_MB; t++)
    {
        for (int g = 0; g < GIRDI_SAYISI; g++)
        {
            unsigned int fiziksel_adres = (t * GIRDI_SAYISI + g) * SAYFA_BOYUTU;
            sayfa_tablolari[t][g] = fiziksel_adres | 0x3;
        }
        sayfa_dizini[t] = ((unsigned int)sayfa_tablolari[t]) | 0x3;
    }

    for (int t = ESLENEN_MB; t < GIRDI_SAYISI; t++)
        sayfa_dizini[t] = 0x2;

    sayfalama_etkinlestir(sayfa_dizini);
}
