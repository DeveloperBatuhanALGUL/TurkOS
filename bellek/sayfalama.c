/*
 * TurkOS - Sayfalama Modulu / Paging Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "sayfalama.h"
#include "../çekirdek/islemci.h"
#include "../sürücüler/görüntü.h"

#define SAYFA_BOYUTU 4096
#define GIRDI_SAYISI 512
#define ESLENEN_ALAN_MB 16
#define SAYFA_TABLOSU_SAYISI (ESLENEN_ALAN_MB / 2)
#define ESLENEN_ALAN_SONU (ESLENEN_ALAN_MB * 1024 * 1024)
#define KORUMA_SAYFASI_LIMITI 16

#define BAYRAK_VAR 0x1ULL
#define BAYRAK_YAZILIR 0x2ULL
#define BAYRAK_CALISTIRILAMAZ (1ULL << 63)

typedef unsigned long long sayfa_girdisi_t;

static sayfa_girdisi_t dizin_isaretci_tablosu[4] __attribute__((aligned(32)));
static sayfa_girdisi_t sayfa_dizini[GIRDI_SAYISI] __attribute__((aligned(4096)));
static sayfa_girdisi_t sayfa_tablolari[SAYFA_TABLOSU_SAYISI][GIRDI_SAYISI] __attribute__((aligned(4096)));
static int nx_etkin = 0;
static unsigned int koruma_sayfalari[KORUMA_SAYFASI_LIMITI];
static unsigned int koruma_sayfasi_sayisi = 0;

extern char cekirdek_metin_baslangici[];
extern char cekirdek_metin_sonu[];
extern char cekirdek_salt_okunur_sonu[];
extern void sayfalama_etkinlestir(unsigned int isaretci_tablosu_adresi, int nx_ac);

static __attribute__((noreturn)) void sistemi_durdur(const char *neden)
{
    goruntu_yaz(neden);
    for (;;)
        __asm__ volatile ("cli; hlt");
}

static sayfa_girdisi_t sayfa_bayragi(unsigned int adres)
{
    sayfa_girdisi_t nx = nx_etkin ? BAYRAK_CALISTIRILAMAZ : 0;

    if (adres < SAYFA_BOYUTU)
        return 0;
    if (adres >= (unsigned int)cekirdek_metin_baslangici && adres < (unsigned int)cekirdek_metin_sonu)
        return BAYRAK_VAR;
    if (adres >= (unsigned int)cekirdek_metin_sonu && adres < (unsigned int)cekirdek_salt_okunur_sonu)
        return BAYRAK_VAR | nx;
    return BAYRAK_VAR | BAYRAK_YAZILIR | nx;
}

int sayfalama_nx_etkin(void)
{
    return nx_etkin;
}

int sayfalama_koruma_sayfasi_kur(unsigned int adres)
{
    if (adres & (SAYFA_BOYUTU - 1))
        return 0;
    if (adres < SAYFA_BOYUTU || adres >= ESLENEN_ALAN_SONU)
        return 0;
    if (!(sayfa_bayragi(adres) & BAYRAK_YAZILIR))
        return 0;
    if (koruma_sayfasi_sayisi >= KORUMA_SAYFASI_LIMITI || sayfalama_koruma_sayfasi_mi(adres))
        return 0;

    unsigned int sira = adres / SAYFA_BOYUTU;
    sayfa_tablolari[sira / GIRDI_SAYISI][sira % GIRDI_SAYISI] = 0;
    __asm__ volatile ("invlpg (%0)" : : "r"(adres) : "memory");
    koruma_sayfalari[koruma_sayfasi_sayisi++] = adres;
    return 1;
}

int sayfalama_koruma_sayfasi_mi(unsigned int adres)
{
    unsigned int sayfa = adres & ~(unsigned int)(SAYFA_BOYUTU - 1);

    for (unsigned int i = 0; i < koruma_sayfasi_sayisi; i++)
        if (koruma_sayfalari[i] == sayfa)
            return 1;
    return 0;
}

void sayfalama_baslat(void)
{
    if (!islemci_pae_destekli())
        sistemi_durdur("\n*** SAYFALAMA: PAE YOK, SISTEM DURDURULDU ***\n");

    nx_etkin = islemci_nx_destekli();

    for (int t = 0; t < SAYFA_TABLOSU_SAYISI; t++)
    {
        for (int g = 0; g < GIRDI_SAYISI; g++)
        {
            unsigned int adres = (unsigned int)(t * GIRDI_SAYISI + g) * SAYFA_BOYUTU;
            sayfa_tablolari[t][g] = (sayfa_girdisi_t)adres | sayfa_bayragi(adres);
        }
        sayfa_dizini[t] = (sayfa_girdisi_t)(unsigned int)sayfa_tablolari[t] | BAYRAK_VAR | BAYRAK_YAZILIR;
    }

    for (int t = SAYFA_TABLOSU_SAYISI; t < GIRDI_SAYISI; t++)
        sayfa_dizini[t] = 0;

    dizin_isaretci_tablosu[0] = (sayfa_girdisi_t)(unsigned int)sayfa_dizini | BAYRAK_VAR;
    for (int i = 1; i < 4; i++)
        dizin_isaretci_tablosu[i] = 0;

    sayfalama_etkinlestir((unsigned int)dizin_isaretci_tablosu, nx_etkin);
}
