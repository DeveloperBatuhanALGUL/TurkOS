#include "../sürücüler/görüntü.h"
#include "../sürücüler/klavye.h"
#include "../donanım/kkt/kesme_kapisi.h"
#include "../donanım/zaman/zamanlayıcı.h"

void cekirdek_ana(void)
{
    goruntu_temizle();
    goruntu_yaz("TurkOS Cekirdek Baslatildi\n");
    goruntu_yaz("BATUSS - Batuhan ALGUL\n");

    idt_baslat();
    klavye_baslat();
    zamanlayici_baslat(100);

    __asm__ volatile ("sti");

    for (;;)
        ;
}
