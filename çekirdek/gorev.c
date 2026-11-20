/*
 * TurkOS - Gorev Yonetimi / Task Management
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "gorev.h"
#include "denetim/gorev_denetleyici.h"
#include "denetim/denetim_otobusu.h"
#include "denetim/devriye.h"

#define GOREV_HAVUZ_LIMITI 8
#define GOREV_EN_KUCUK_YIGIN 256
#define GOREV_OLUSTURMA_REDDI 4

extern void gorev_degistir(unsigned int *eski_esp_adresi, unsigned int yeni_esp);
extern __attribute__((noreturn)) void devriye_sistemi_durdur(void);

static gorev_t *simdiki = 0;
static unsigned int baslangic_yigin_esp;
static unsigned int sonraki_id = 0;

gorev_t *gorev_olustur(void (*fonksiyon)(void), unsigned int *yigin, unsigned int boyut)
{
    static gorev_t havuz[GOREV_HAVUZ_LIMITI];
    static unsigned int sayac = 0;

    if (sayac >= GOREV_HAVUZ_LIMITI || !fonksiyon || !yigin || boyut < GOREV_EN_KUCUK_YIGIN)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_GOREV, DENETIM_SEVIYE_ALARM, GOREV_OLUSTURMA_REDDI, sayac);
        return 0;
    }

    gorev_t *g = &havuz[sayac++];
    unsigned int *tepe = yigin + (boyut / sizeof(unsigned int));

    tepe[-1] = (unsigned int)fonksiyon;
    tepe[-2] = 0;
    tepe[-3] = 0;
    tepe[-4] = 0;
    tepe[-5] = 0;

    g->esp = (unsigned int)&tepe[-5];
    g->sonraki = g;
    g->durum = GOREV_DURUM_CALISIYOR;
    g->id = sonraki_id++;

    return g;
}

void gorev_zincirle(gorev_t *birinci, gorev_t *ikinci)
{
    birinci->sonraki = ikinci;
}

void gorev_baslat(gorev_t *ilk_gorev)
{
    simdiki = ilk_gorev;
    gorev_degistir(&baslangic_yigin_esp, ilk_gorev->esp);
}

static int calisabilir_mi(gorev_t *g)
{
    return g->durum != GOREV_DURUM_KARANTINA && g->durum != GOREV_DURUM_SONLANDIRILDI;
}

static gorev_t *calisabilir_sonraki_bul(gorev_t *baslangic)
{
    gorev_t *aday = baslangic->sonraki;

    for (unsigned int tur = 0; tur < GOREV_HAVUZ_LIMITI; tur++)
    {
        if (calisabilir_mi(aday))
            return aday;
        if (aday == baslangic)
            return 0;
        aday = aday->sonraki;
    }

    return 0;
}

void gorev_yield(void)
{
    if (devriye_kilitlendi_mi())
        devriye_sistemi_durdur();

    gorev_t *eski = simdiki;
    gorev_t *yeni = calisabilir_sonraki_bul(eski);

    if (!yeni)
        devriye_sistemi_durdur();

    if (yeni == eski)
        return;

    simdiki = yeni;
    gorev_denetleyici_gecis_bildir(eski, yeni);
    gorev_degistir(&eski->esp, yeni->esp);
}

void gorev_karantinaya_al(gorev_t *g)
{
    if (g->durum == GOREV_DURUM_KARANTINA || g->durum == GOREV_DURUM_SONLANDIRILDI)
        return;

    g->durum = GOREV_DURUM_KARANTINA;
}

void gorev_sonlandir(gorev_t *g)
{
    g->durum = GOREV_DURUM_SONLANDIRILDI;
}

gorev_t *gorev_su_anki(void)
{
    return simdiki;
}
