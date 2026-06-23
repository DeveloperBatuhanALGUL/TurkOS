#include "kullanici_modu.h"

#define SISTEM_CAGRISI_YAZ 0

static inline void sistem_cagrisi_yap(unsigned int numara, unsigned int arg)
{
    __asm__ volatile (
        "int $0x80"
        :
        : "a"(numara), "b"(arg)
    );
}

void kullanici_testi(void)
{
    sistem_cagrisi_yap(SISTEM_CAGRISI_YAZ, (unsigned int)"Ring3'ten Sistem Cagrisi Basarili!\n");

    for (;;)
        ;
}
