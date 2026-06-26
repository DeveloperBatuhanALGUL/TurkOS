#include "devriye.h"
#include "denetim_otobusu.h"
#include "bellek_denetleyici.h"

#define DEVRIYE_PERIYODU 50
#define KILITLEME_ESIGI 3

static unsigned int devriye_tik_sayaci = 0;
static unsigned int devriye_tur_sayisi = 0;
static unsigned int son_kontrol_alarm_sayisi = 0;
static unsigned int sistem_kilitli = 0;

void devriye_baslat(void)
{
    devriye_tik_sayaci = 0;
    devriye_tur_sayisi = 0;
    son_kontrol_alarm_sayisi = 0;
    sistem_kilitli = 0;
}

unsigned int devriye_kilitlendi_mi(void)
{
    return sistem_kilitli;
}

void devriye_tik_bildir(void)
{
    if (sistem_kilitli)
        return;

    devriye_tik_sayaci++;

    if (devriye_tik_sayaci < DEVRIYE_PERIYODU)
        return;

    devriye_tik_sayaci = 0;
    devriye_tur_sayisi++;

    unsigned int simdiki_alarm_sayisi = denetim_alarm_sayisi();
    unsigned int bu_turdaki_yeni_alarmlar = simdiki_alarm_sayisi - son_kontrol_alarm_sayisi;
    son_kontrol_alarm_sayisi = simdiki_alarm_sayisi;

    denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_BILGI, devriye_tur_sayisi, simdiki_alarm_sayisi);

    if (bu_turdaki_yeni_alarmlar >= KILITLEME_ESIGI)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_DEVRIYE, DENETIM_SEVIYE_ALARM, devriye_tur_sayisi, bu_turdaki_yeni_alarmlar);
        sistem_kilitli = 1;
    }
}
