#include "../sürücüler/görüntü.h"
#include "../sürücüler/klavye.h"
#include "../donanım/kkt/kesme_kapisi.h"

void cekirdek_ana(void)
{
    goruntu_temizle();
    goruntu_yaz("TurkOS Cekirdek Baslatildi\n");
    goruntu_yaz("BATUSS - Batuhan ALGUL\n");

    idt_baslat();
    klavye_baslat();

    __asm__ volatile ("sti");

    for (;;)
        ;
}
