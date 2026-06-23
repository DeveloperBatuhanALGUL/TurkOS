#include "gorev_denetleyici.h"
#include "denetim_otobusu.h"

#define GOREV_LIMITI 8
#define ESIK_GECIS_SAYISI 0xFFFFFFF0

static gorev_t *izlenen_gorevler[GOREV_LIMITI];
static unsigned int gecis_sayaclari[GOREV_LIMITI];
static unsigned int izlenen_gorev_sayisi = 0;

static int gorev_dizini_bul(gorev_t *g)
{
    for (unsigned int i = 0; i < izlenen_gorev_sayisi; i++)
        if (izlenen_gorevler[i] == g)
            return (int)i;
    return -1;
}

void gorev_denetleyici_baslat(void)
{
    izlenen_gorev_sayisi = 0;
}

void gorev_denetleyici_gecis_bildir(gorev_t *eski, gorev_t *yeni)
{
    if (eski == yeni)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_SUPHELI, 1, (unsigned int)(unsigned long)yeni);
        return;
    }

    int dizin = gorev_dizini_bul(yeni);
    if (dizin < 0)
    {
        if (izlenen_gorev_sayisi < GOREV_LIMITI)
        {
            dizin = (int)izlenen_gorev_sayisi;
            izlenen_gorevler[dizin] = yeni;
            gecis_sayaclari[dizin] = 0;
            izlenen_gorev_sayisi++;
        }
        else
        {
            denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_ALARM, 2, izlenen_gorev_sayisi);
            return;
        }
    }

    gecis_sayaclari[dizin]++;

    if (gecis_sayaclari[dizin] >= ESIK_GECIS_SAYISI)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_ALARM, 3, gecis_sayaclari[dizin]);
        gecis_sayaclari[dizin] = 0;
    }
}
