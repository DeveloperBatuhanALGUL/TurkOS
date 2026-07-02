#include "gorev.h"
#include "denetim/gorev_denetleyici.h"
#include "panik.h"

extern void gorev_degistir(unsigned int *eski_esp_adresi, unsigned int yeni_esp);

static gorev_t *simdiki = 0;
static unsigned int baslangic_yigin_esp;
static unsigned int sonraki_id = 0;

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

static gorev_t *calisabilir_sonraki_bul(gorev_t *baslangic)
{
    gorev_t *aday = baslangic->sonraki;
    unsigned int tur = 0;

    while (aday->durum == GOREV_DURUM_KARANTINA || aday->durum == GOREV_DURUM_SONLANDIRILDI)
    {
        aday = aday->sonraki;
        tur++;
        if (tur > 8)
            panik("Calisabilir gorev bulunamadi - tum gorevler karantinada", tur);
    }

    return aday;
}

void gorev_yield(void)
{
    gorev_t *eski = simdiki;
    gorev_t *yeni = calisabilir_sonraki_bul(simdiki);
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
