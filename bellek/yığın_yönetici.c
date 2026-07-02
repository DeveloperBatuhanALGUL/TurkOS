#include "yığın_yönetici.h"
#include "../çekirdek/denetim/bellek_denetleyici.h"

#define YIGIN_BOYUTU (4 * 1024 * 1024)

struct blok_basligi {
    unsigned int boyut;
    int kullanimda;
    struct blok_basligi *sonraki;
};

static unsigned char yigin_alani[YIGIN_BOYUTU] __attribute__((aligned(4096)));
static struct blok_basligi *ilk_blok = 0;

void yigin_baslat(void)
{
    ilk_blok = (struct blok_basligi *)yigin_alani;
    ilk_blok->boyut = YIGIN_BOYUTU - sizeof(struct blok_basligi);
    ilk_blok->kullanimda = 0;
    ilk_blok->sonraki = 0;
}

void *kmalloc(unsigned int boyut)
{
    struct blok_basligi *gezici = ilk_blok;

    while (gezici)
    {
        if (!gezici->kullanimda && gezici->boyut >= boyut)
        {
            unsigned int kalan = gezici->boyut - boyut;

            if (kalan > sizeof(struct blok_basligi))
            {
                struct blok_basligi *yeni_blok =
                    (struct blok_basligi *)((unsigned char *)gezici + sizeof(struct blok_basligi) + boyut);
                yeni_blok->boyut = kalan - sizeof(struct blok_basligi);
                yeni_blok->kullanimda = 0;
                yeni_blok->sonraki = gezici->sonraki;

                gezici->boyut = boyut;
                gezici->sonraki = yeni_blok;
            }

            gezici->kullanimda = 1;
            void *adres = (unsigned char *)gezici + sizeof(struct blok_basligi);
            bellek_denetleyici_tahsis_bildir(adres, boyut);
            return adres;
        }
        gezici = gezici->sonraki;
    }

    bellek_denetleyici_tahsis_bildir(0, boyut);
    return 0;
}

static void bloklari_birlestir(void)
{
    struct blok_basligi *gezici = ilk_blok;

    while (gezici && gezici->sonraki)
    {
        if (!gezici->kullanimda && !gezici->sonraki->kullanimda)
        {
            gezici->boyut += sizeof(struct blok_basligi) + gezici->sonraki->boyut;
            gezici->sonraki = gezici->sonraki->sonraki;
        }
        else
        {
            gezici = gezici->sonraki;
        }
    }
}

void kfree(void *blok)
{
    if (!blok)
        return;

    if (!bellek_denetleyici_serbest_izinli_mi(blok))
    {
        bellek_denetleyici_serbest_bildir(blok);
        return;
    }

    bellek_denetleyici_serbest_bildir(blok);

    struct blok_basligi *baslik =
        (struct blok_basligi *)((unsigned char *)blok - sizeof(struct blok_basligi));
    baslik->kullanimda = 0;

    bloklari_birlestir();
}
