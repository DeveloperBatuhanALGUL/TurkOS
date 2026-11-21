#include "kullanici_modu.h"

#define KULLANICI_METIN __attribute__((section(".kullanici_metin"), noinline))
#define KULLANICI_VERI __attribute__((section(".kullanici_veri")))
#define SISTEM_CAGRISI_YAZ 0

KULLANICI_VERI static char kullanici_mesaji[] = "Ring3'ten Sistem Cagrisi Basarili!\n";
KULLANICI_VERI __attribute__((aligned(16))) unsigned char kullanici_yigini[4096];

KULLANICI_METIN void kullanici_testi(void)
{
    __asm__ volatile ("int $0x80" : : "a"(SISTEM_CAGRISI_YAZ), "b"(kullanici_mesaji));

    for (;;)
        ;
}
