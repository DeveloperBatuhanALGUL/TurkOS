#include "../sürücüler/görüntü.h"
#include "gdt.h"
#include "../sürücüler/klavye.h"
#include "../donanım/kkt/kesme_kapisi.h"
#include "../donanım/zaman/zamanlayıcı.h"
#include "../bellek/sayfalama.h"
#include "../bellek/yığın_yönetici.h"

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

    for (;;)
        ;
}
