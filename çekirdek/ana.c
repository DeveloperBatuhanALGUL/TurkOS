#include "../sürücüler/görüntü.h"
#include "gdt.h"
#include "islemci.h"
#include "../sürücüler/klavye.h"
#include "../donanım/kkt/kesme_kapisi.h"
#include "../donanım/zaman/zamanlayıcı.h"
#include "../bellek/sayfalama.h"
#include "../bellek/yığın_yönetici.h"
#include "kullanici_modu.h"
#include "gorev.h"
#include "denetim/denetim_otobusu.h"
#include "denetim/gorev_denetleyici.h"
#include "denetim/bellek_denetleyici.h"
#include "denetim/kesme_denetleyici.h"
#include "denetim/devriye.h"

#define SAYFA_BOYUTU 4096

extern char yigin_koruma[];

static unsigned char gorev_a_alani[2 * SAYFA_BOYUTU] __attribute__((aligned(SAYFA_BOYUTU)));
static unsigned char gorev_b_alani[2 * SAYFA_BOYUTU] __attribute__((aligned(SAYFA_BOYUTU)));

#ifdef TURKOS_TEST_YIGIN_TASMASI
static unsigned int yigin_tasir(unsigned int derinlik)
{
    volatile unsigned char tampon[256];

    tampon[0] = (unsigned char)derinlik;
    if (derinlik == 0xFFFFFFFF)
        return 0;
    return yigin_tasir(derinlik + 1) + tampon[0];
}
#endif

#ifdef TURKOS_TEST_YIGIN_BOZMA
static void yigin_boz(void)
{
    char tampon[16];
    volatile char *yazici = tampon;

    for (int i = 0; i < 48; i++)
        yazici[i] = 0x41;
}
#endif

void gorev_a_fonksiyon(void)
{
    int sayac = 0;
#ifdef TURKOS_TEST_YIGIN_TASMASI
    goruntu_yaz("TEST: yigin tasmasi deneniyor\n");
    yigin_tasir(0);
#endif
#ifdef TURKOS_TEST_YIGIN_BOZMA
    goruntu_yaz("TEST: yigin bozma deneniyor\n");
    yigin_boz();
    goruntu_yaz("TEST: BOZMA YAKALANMADI\n");
#endif
    for (;;)
    {
        goruntu_yaz("A");
        gorev_yield();
        sayac++;
        if (sayac == 5)
        {
            void *test_blok = kmalloc(64);
            kfree(test_blok);
            kfree(test_blok);
            kfree(test_blok);
            kfree(test_blok);

            for (int i = 0; i < 6; i++)
                kesme_denetleyici_istisna_bildir(14, 0);

            denetim_gunlugu_yazdir();
            goruntu_yaz("Alarm Sayisi: ");
            goruntu_yaz_hex(denetim_alarm_sayisi());
            goruntu_yaz("\n");
        }
        goruntu_yaz("[T:");
        goruntu_yaz_hex(zamanlayici_tik_sayisi());
        goruntu_yaz("]");
        if (devriye_kilitlendi_mi())
        {
            goruntu_yaz("\n*** DEVRIYE: SISTEM KILITLENDI ***\n");
            for (;;)
                ;
        }
    }
}

void gorev_b_fonksiyon(void)
{
    for (;;)
    {
        goruntu_yaz("B");
        gorev_yield();
    }
}

void cekirdek_ana(void)
{
    gdt_baslat();
    goruntu_temizle();
    goruntu_yaz("TurkOS Cekirdek Baslatildi\n");
    goruntu_yaz("GDT ve TSS Hazir\n");
    goruntu_yaz("BATUSS - Batuhan ALGUL\n");

    goruntu_yaz(islemci_pae_destekli() ? "Islemci: PAE var\n" : "Islemci: PAE YOK\n");
    goruntu_yaz(islemci_nx_destekli() ? "Islemci: NX var\n" : "Islemci: NX YOK\n");
    sayfalama_baslat();
    goruntu_yaz(sayfalama_nx_etkin() ? "Sayfalama: PAE ve NX Etkin\n" : "Sayfalama: PAE Etkin, NX YOK\n");
    unsigned int koruma_sayisi = sayfalama_koruma_sayfasi_kur((unsigned int)yigin_koruma)
        + sayfalama_koruma_sayfasi_kur((unsigned int)gorev_a_alani)
        + sayfalama_koruma_sayfasi_kur((unsigned int)gorev_b_alani);
    goruntu_yaz("Koruma Sayfalari: ");
    goruntu_yaz_hex(koruma_sayisi);
    goruntu_yaz("\n");
    cift_hata_gorevi_hazirla();
    goruntu_yaz("Cift Hata Gorevi Hazir\n");

    yigin_baslat();
    goruntu_yaz("Yigin Hazir\n");

    idt_baslat();
    klavye_baslat();
    zamanlayici_baslat(100);

    __asm__ volatile ("sti");

    denetim_otobusu_baslat();
    gorev_denetleyici_baslat();
    bellek_denetleyici_baslat();
    kesme_denetleyici_baslat();
    devriye_baslat();

    goruntu_yaz("Gorev Degisimi Deneniyor: ");

    gorev_t *gorev_a = gorev_olustur(gorev_a_fonksiyon, (unsigned int *)(gorev_a_alani + SAYFA_BOYUTU), SAYFA_BOYUTU);
    gorev_t *gorev_b = gorev_olustur(gorev_b_fonksiyon, (unsigned int *)(gorev_b_alani + SAYFA_BOYUTU), SAYFA_BOYUTU);

    gorev_zincirle(gorev_a, gorev_b);
    gorev_zincirle(gorev_b, gorev_a);

    gorev_baslat(gorev_a);

    for (;;)
        ;
}
