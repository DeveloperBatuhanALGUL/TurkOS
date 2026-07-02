/*
 * TurkOS - Devriye Modulu / Patrol Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "devriye.h"
#include "denetim_otobusu.h"
#include "bellek_denetleyici.h"
#include "gorev_denetleyici.h"

#define DEVRIYE_PERIYODU 50
#define KILITLEME_ESIGI 3
#define SUPHE_SOGUMA_SURESI 200
#define BUTUNLUK_SABITI 0x42415455
#define KARANTINA_DOYGUNLUK_TABAN 2

static unsigned int devriye_tik_sayaci = 0;
static unsigned int devriye_tur_sayisi = 0;
static unsigned int son_kontrol_alarm_sayisi = 0;
static unsigned int supheli_olay_sayaci = 0;
static unsigned int son_supheli_tik = 0;

static unsigned int sistem_kilitli_a = 0;
static unsigned int sistem_kilitli_b = BUTUNLUK_SABITI;

static void kilitle(void)
{
    sistem_kilitli_a = 1;
    sistem_kilitli_b = BUTUNLUK_SABITI ^ 1;
}

static unsigned int butunluk_bozuldu_mu(void)
{
    unsigned int beklenen_b_kilitli = BUTUNLUK_SABITI ^ 1;
    unsigned int beklenen_b_acik = BUTUNLUK_SABITI;

    if (sistem_kilitli_a == 1 && sistem_kilitli_b == beklenen_b_kilitli)
        return 0;
    if (sistem_kilitli_a == 0 && sistem_kilitli_b == beklenen_b_acik)
        return 0;

    return 1;
}

void devriye_baslat(void)
{
    devriye_tik_sayaci = 0;
    devriye_tur_sayisi = 0;
    son_kontrol_alarm_sayisi = 0;
    supheli_olay_sayaci = 0;
    son_supheli_tik = 0;
    sistem_kilitli_a = 0;
    sistem_kilitli_b = BUTUNLUK_SABITI;
}

unsigned int devriye_kilitlendi_mi(void)
{
    if (butunluk_bozuldu_mu())
    {
        denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_ALARM, 0xBADBAD, sistem_kilitli_a);
        kilitle();
        return 1;
    }

    return sistem_kilitli_a;
}

static void esik_kontrolu_yap(unsigned int tetikleyici_tik)
{
    unsigned int simdiki_alarm_sayisi = denetim_alarm_sayisi();
    unsigned int yeni_alarmlar = simdiki_alarm_sayisi - son_kontrol_alarm_sayisi;
    son_kontrol_alarm_sayisi = simdiki_alarm_sayisi;

    if (yeni_alarmlar == 0)
        return;

    if (tetikleyici_tik - son_supheli_tik > SUPHE_SOGUMA_SURESI)
        supheli_olay_sayaci = 0;

    supheli_olay_sayaci += yeni_alarmlar;
    son_supheli_tik = tetikleyici_tik;

    denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_SUPHELI, tetikleyici_tik, supheli_olay_sayaci);

    if (supheli_olay_sayaci >= KILITLEME_ESIGI)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_ALARM, tetikleyici_tik, supheli_olay_sayaci);
        kilitle();
    }
}

static void doygunluk_kontrolu_yap(unsigned int tetikleyici_tik)
{
    unsigned int aktif = gorev_denetleyici_aktif_sayisi();
    unsigned int karantina = gorev_denetleyici_karantina_sayisi();

    if (aktif == 0)
        return;

    unsigned int esik = aktif / 2;
    if (esik < KARANTINA_DOYGUNLUK_TABAN)
        esik = KARANTINA_DOYGUNLUK_TABAN;

    if (karantina >= esik)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_ALARM, tetikleyici_tik, karantina);
        kilitle();
    }
}

void devriye_olay_bildirildi(void)
{
    if (sistem_kilitli_a)
        return;

    esik_kontrolu_yap(devriye_tik_sayaci + (devriye_tur_sayisi * DEVRIYE_PERIYODU));
}

void devriye_tik_bildir(void)
{
    if (sistem_kilitli_a)
        return;

    doygunluk_kontrolu_yap(devriye_tik_sayaci + (devriye_tur_sayisi * DEVRIYE_PERIYODU));

    devriye_tik_sayaci++;

    if (devriye_tik_sayaci < DEVRIYE_PERIYODU)
        return;

    devriye_tik_sayaci = 0;
    devriye_tur_sayisi++;

    denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_BILGI, devriye_tur_sayisi, denetim_alarm_sayisi());
}
