#include "gorev.h"

extern void gorev_degistir(unsigned int *eski_esp_adresi, unsigned int yeni_esp);

static gorev_t *simdiki = 0;
static unsigned int baslangic_yigin_esp;

gorev_t *gorev_olustur(void (*fonksiyon)(void), unsigned int *yigin, unsigned int boyut)
{
    static gorev_t havuz[8];
    static int sayac = 0;

    gorev_t *g = &havuz[sayac++];
    unsigned int *tepe = yigin + (boyut / sizeof(unsigned int));

    tepe[-1] = (unsigned int)fonksiyon;
    tepe[-2] = 0;
    tepe[-3] = 0;
    tepe[-4] = 0;
    tepe[-5] = 0;

    g->esp = (unsigned int)&tepe[-5];
    g->sonraki = g;

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

void gorev_yield(void)
{
    gorev_t *eski = simdiki;
    gorev_t *yeni = simdiki->sonraki;
    simdiki = yeni;

    gorev_degistir(&eski->esp, yeni->esp);
}
