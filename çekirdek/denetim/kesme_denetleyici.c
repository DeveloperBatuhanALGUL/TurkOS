#include "kesme_denetleyici.h"
#include "denetim_otobusu.h"

#define ISTISNA_LIMITI 32
#define ESIK_TEKRAR_SAYISI 5

static unsigned int istisna_sayaclari[ISTISNA_LIMITI];
static unsigned int son_istisna_numarasi = 0xFFFFFFFF;
static unsigned int ust_uste_sayac = 0;

void kesme_denetleyici_baslat(void)
{
    for (unsigned int i = 0; i < ISTISNA_LIMITI; i++)
        istisna_sayaclari[i] = 0;
    son_istisna_numarasi = 0xFFFFFFFF;
    ust_uste_sayac = 0;
}

void kesme_denetleyici_istisna_bildir(unsigned int numara, unsigned int kod)
{
    if (numara < ISTISNA_LIMITI)
        istisna_sayaclari[numara]++;

    if (numara == son_istisna_numarasi)
    {
        ust_uste_sayac++;
        if (ust_uste_sayac >= ESIK_TEKRAR_SAYISI)
        {
            denetim_olay_bildir(DENETIM_KAYNAK_KESME, DENETIM_SEVIYE_ALARM, numara, kod);
            ust_uste_sayac = 0;
        }
        else
        {
            denetim_olay_bildir(DENETIM_KAYNAK_KESME, DENETIM_SEVIYE_SUPHELI, numara, kod);
        }
    }
    else
    {
        son_istisna_numarasi = numara;
        ust_uste_sayac = 1;
        denetim_olay_bildir(DENETIM_KAYNAK_KESME, DENETIM_SEVIYE_BILGI, numara, kod);
    }
}
