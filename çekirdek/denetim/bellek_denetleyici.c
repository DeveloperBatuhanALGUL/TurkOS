#include "bellek_denetleyici.h"
#include "denetim_otobusu.h"
#include "gorev_denetleyici.h"
#include "../gorev.h"

#define TAKIP_LIMITI 64
#define ZEHIRLI_HAVUZ_LIMITI 32

static void *takip_adresleri[TAKIP_LIMITI];
static unsigned int takip_boyutlari[TAKIP_LIMITI];
static unsigned int takip_dolu[TAKIP_LIMITI];
static unsigned int acik_tahsis_sayisi = 0;
static unsigned int toplam_tahsis_edilen = 0;

static void *zehirli_havuz[ZEHIRLI_HAVUZ_LIMITI];
static unsigned int zehirli_sayisi = 0;

void bellek_denetleyici_baslat(void)
{
    for (unsigned int i = 0; i < TAKIP_LIMITI; i++)
        takip_dolu[i] = 0;
    acik_tahsis_sayisi = 0;
    toplam_tahsis_edilen = 0;
    zehirli_sayisi = 0;
}

static int bos_yuva_bul(void)
{
    for (unsigned int i = 0; i < TAKIP_LIMITI; i++)
        if (!takip_dolu[i])
            return (int)i;
    return -1;
}

static int adres_yuvasi_bul(void *adres)
{
    for (unsigned int i = 0; i < TAKIP_LIMITI; i++)
        if (takip_dolu[i] && takip_adresleri[i] == adres)
            return (int)i;
    return -1;
}

static void zehirli_havuza_ekle(void *adres)
{
    if (zehirli_sayisi < ZEHIRLI_HAVUZ_LIMITI)
        zehirli_havuz[zehirli_sayisi++] = adres;
}

static void ilgili_gorevi_bildir(void)
{
    gorev_t *g = gorev_su_anki();
    if (g)
        gorev_denetleyici_supheli_bildir(g);
}

void bellek_denetleyici_tahsis_bildir(void *adres, unsigned int boyut)
{
    if (!adres)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_BELLEK, DENETIM_SEVIYE_SUPHELI, 1, boyut);
        return;
    }

    int yuva = bos_yuva_bul();
    if (yuva < 0)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_BELLEK, DENETIM_SEVIYE_ALARM, 2, TAKIP_LIMITI);
        ilgili_gorevi_bildir();
        return;
    }

    takip_adresleri[yuva] = adres;
    takip_boyutlari[yuva] = boyut;
    takip_dolu[yuva] = 1;
    acik_tahsis_sayisi++;
    toplam_tahsis_edilen++;
}

int bellek_denetleyici_serbest_izinli_mi(void *adres)
{
    if (!adres)
        return 0;

    return adres_yuvasi_bul(adres) >= 0;
}

void bellek_denetleyici_serbest_bildir(void *adres)
{
    if (!adres)
        return;

    int yuva = adres_yuvasi_bul(adres);
    if (yuva < 0)
    {
        denetim_olay_bildir(DENETIM_KAYNAK_BELLEK, DENETIM_SEVIYE_ALARM, 3, (unsigned int)(unsigned long)adres);
        zehirli_havuza_ekle(adres);
        ilgili_gorevi_bildir();
        return;
    }

    takip_dolu[yuva] = 0;
    acik_tahsis_sayisi--;
}

unsigned int bellek_denetleyici_acik_tahsis_sayisi(void)
{
    return acik_tahsis_sayisi;
}

unsigned int bellek_denetleyici_zehirli_sayisi(void)
{
    return zehirli_sayisi;
}
