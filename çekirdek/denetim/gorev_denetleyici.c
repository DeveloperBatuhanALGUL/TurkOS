/*
 * TurkOS - Gorev Denetleyici Modulu / Task Supervisor Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "gorev_denetleyici.h"
#include "denetim_otobusu.h"

#define GOREV_LIMITI 8
#define ESIK_GECIS_SAYISI 0xFFFFFFF0
#define SUPHE_KARANTINA_ESIGI 3

static gorev_t *izlenen_gorevler[GOREV_LIMITI];
static unsigned int gecis_sayaclari[GOREV_LIMITI];
static unsigned int supheli_sayaclari[GOREV_LIMITI];
static unsigned int izlenen_gorev_sayisi = 0;

static int gorev_dizini_bul(gorev_t *g)
{
    for (unsigned int i = 0; i < izlenen_gorev_sayisi; i++)
        if (izlenen_gorevler[i] == g)
            return (int)i;
    return -1;
}

static int gorev_dizini_kaydet(gorev_t *g)
{
    if (izlenen_gorev_sayisi >= GOREV_LIMITI)
        return -1;

    int dizin = (int)izlenen_gorev_sayisi;
    izlenen_gorevler[dizin] = g;
    gecis_sayaclari[dizin] = 0;
    supheli_sayaclari[dizin] = 0;
    izlenen_gorev_sayisi++;
    return dizin;
}

void gorev_denetleyici_baslat(void)
{
    izlenen_gorev_sayisi = 0;
}

void gorev_denetleyici_gecis_bildir(gorev_t *eski, gorev_t *yeni)
{
    if (gorev_dizini_bul(eski) < 0)
        gorev_dizini_kaydet(eski);

    if (eski == yeni)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_ALARM, 1, (unsigned int)(unsigned long)yeni);
        gorev_karantinaya_al(yeni);
        return;
    }

    int dizin = gorev_dizini_bul(yeni);
    if (dizin < 0)
    {
        dizin = gorev_dizini_kaydet(yeni);
        if (dizin < 0)
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

void gorev_denetleyici_supheli_bildir(gorev_t *g)
{
    int dizin = gorev_dizini_bul(g);
    if (dizin < 0)
        dizin = gorev_dizini_kaydet(g);
    if (dizin < 0)
        return;

    supheli_sayaclari[dizin]++;
    denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_SUPHELI, g->id, supheli_sayaclari[dizin]);

    if (supheli_sayaclari[dizin] >= SUPHE_KARANTINA_ESIGI)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_ALARM, g->id, supheli_sayaclari[dizin]);
        gorev_karantinaya_al(g);
        supheli_sayaclari[dizin] = 0;
    }
}

unsigned int gorev_denetleyici_aktif_sayisi(void)
{
    unsigned int sayac = 0;
    for (unsigned int i = 0; i < izlenen_gorev_sayisi; i++)
        if (izlenen_gorevler[i]->durum != GOREV_DURUM_SONLANDIRILDI)
            sayac++;
    return sayac;
}

unsigned int gorev_denetleyici_karantina_sayisi(void)
{
    unsigned int sayac = 0;
    for (unsigned int i = 0; i < izlenen_gorev_sayisi; i++)
        if (izlenen_gorevler[i]->durum == GOREV_DURUM_KARANTINA)
            sayac++;
    return sayac;
}
