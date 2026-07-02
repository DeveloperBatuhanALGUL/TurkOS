/*
 * TurkOS - Sayfalama Modulu / Paging Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "sayfalama.h"

#define GIRDI_SAYISI 1024
#define SAYFA_BOYUTU 4096
#define ESLENEN_MB 16
#define CEKIRDEK_SAYFA_BAYRAK 0x3
#define BOS_GIRDI_BAYRAK 0x2

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
            sayfa_tablolari[t][g] = fiziksel_adres | CEKIRDEK_SAYFA_BAYRAK;
        }
        sayfa_dizini[t] = ((unsigned int)sayfa_tablolari[t]) | CEKIRDEK_SAYFA_BAYRAK;
    }

    for (int t = ESLENEN_MB; t < GIRDI_SAYISI; t++)
        sayfa_dizini[t] = BOS_GIRDI_BAYRAK;

    sayfalama_etkinlestir(sayfa_dizini);
}
