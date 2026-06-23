#include "denetim_otobusu.h"

#define GUNLUK_BOYUTU 64
#define DINLEYICI_LIMITI 8

static denetim_olay_t gunluk[GUNLUK_BOYUTU];
static unsigned int gunluk_yazma_konumu = 0;
static unsigned int gunluk_dolu_sayisi = 0;
static unsigned int alarm_sayisi = 0;

static denetim_dinleyici_t dinleyiciler[DINLEYICI_LIMITI];
static unsigned int dinleyici_sayisi = 0;

void denetim_otobusu_baslat(void)
{
    gunluk_yazma_konumu = 0;
    gunluk_dolu_sayisi = 0;
    alarm_sayisi = 0;
    dinleyici_sayisi = 0;
}

void denetim_dinleyici_kaydet(denetim_dinleyici_t dinleyici)
{
    if (dinleyici_sayisi < DINLEYICI_LIMITI)
        dinleyiciler[dinleyici_sayisi++] = dinleyici;
}

void denetim_olay_bildir(denetim_kaynak_t kaynak, denetim_seviye_t seviye, unsigned int kod, unsigned int detay)
{
    denetim_olay_t *olay = &gunluk[gunluk_yazma_konumu];
    olay->kaynak = kaynak;
    olay->seviye = seviye;
    olay->kod = kod;
    olay->detay = detay;

    gunluk_yazma_konumu = (gunluk_yazma_konumu + 1) % GUNLUK_BOYUTU;
    if (gunluk_dolu_sayisi < GUNLUK_BOYUTU)
        gunluk_dolu_sayisi++;

    if (seviye == DENETIM_SEVIYE_ALARM)
        alarm_sayisi++;

    for (unsigned int i = 0; i < dinleyici_sayisi; i++)
        dinleyiciler[i](olay);
}

unsigned int denetim_alarm_sayisi(void)
{
    return alarm_sayisi;
}

extern void goruntu_yaz(const char *metin);
extern void goruntu_yaz_hex(unsigned int deger);

static const char *kaynak_adi(denetim_kaynak_t kaynak)
{
    switch (kaynak)
    {
        case DENETIM_KAYNAK_GOREV: return "GOREV";
        case DENETIM_KAYNAK_BELLEK: return "BELLEK";
        case DENETIM_KAYNAK_KESME: return "KESME";
        case DENETIM_KAYNAK_SISTEM_CAGRISI: return "SISTEM_CAGRISI";
        default: return "BILINMEYEN";
    }
}

static const char *seviye_adi(denetim_seviye_t seviye)
{
    switch (seviye)
    {
        case DENETIM_SEVIYE_BILGI: return "BILGI";
        case DENETIM_SEVIYE_SUPHELI: return "SUPHELI";
        case DENETIM_SEVIYE_ALARM: return "ALARM";
        default: return "BILINMEYEN";
    }
}

void denetim_gunlugu_yazdir(void)
{
    goruntu_yaz("\n--- DENETIM GUNLUGU ---\n");
    unsigned int baslangic = (gunluk_dolu_sayisi < GUNLUK_BOYUTU) ? 0 : gunluk_yazma_konumu;

    for (unsigned int i = 0; i < gunluk_dolu_sayisi; i++)
    {
        unsigned int konum = (baslangic + i) % GUNLUK_BOYUTU;
        denetim_olay_t *olay = &gunluk[konum];

        goruntu_yaz("[");
        goruntu_yaz(kaynak_adi(olay->kaynak));
        goruntu_yaz("/");
        goruntu_yaz(seviye_adi(olay->seviye));
        goruntu_yaz("] kod=");
        goruntu_yaz_hex(olay->kod);
        goruntu_yaz(" detay=");
        goruntu_yaz_hex(olay->detay);
        goruntu_yaz("\n");
    }
}
