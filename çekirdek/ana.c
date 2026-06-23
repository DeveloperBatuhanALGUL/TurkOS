#include "../sürücüler/görüntü.h"
#include "gdt.h"
#include "../sürücüler/klavye.h"
#include "../donanım/kkt/kesme_kapisi.h"
#include "../donanım/zaman/zamanlayıcı.h"
#include "../bellek/sayfalama.h"
#include "../bellek/yığın_yönetici.h"
#include "kullanici_modu.h"
#include "gorev.h"

static unsigned char gorev_a_yigini[4096] __attribute__((aligned(16)));
static unsigned char gorev_b_yigini[4096] __attribute__((aligned(16)));

void gorev_a_fonksiyon(void)
{
    for (;;)
    {
        goruntu_yaz("A");
        gorev_yield();
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

    sayfalama_baslat();
    goruntu_yaz("Sayfalama Etkin\n");

    yigin_baslat();
    goruntu_yaz("Yigin Hazir\n");

    idt_baslat();
    klavye_baslat();
    zamanlayici_baslat(100);

    __asm__ volatile ("sti");

    goruntu_yaz("Gorev Degisimi Deneniyor: ");

    gorev_t *gorev_a = gorev_olustur(gorev_a_fonksiyon, (unsigned int *)gorev_a_yigini, sizeof(gorev_a_yigini));
    gorev_t *gorev_b = gorev_olustur(gorev_b_fonksiyon, (unsigned int *)gorev_b_yigini, sizeof(gorev_b_yigini));

    gorev_zincirle(gorev_a, gorev_b);
    gorev_zincirle(gorev_b, gorev_a);

    gorev_baslat(gorev_a);

    for (;;)
        ;
}
