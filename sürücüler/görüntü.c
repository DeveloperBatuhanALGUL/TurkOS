#include "görüntü.h"

static volatile unsigned short *vga_bellek = (unsigned short *)0xB8000;
static int satir = 0;
static int sutun = 0;
static const int GENISLIK = 80;
static const int YUKSEKLIK = 25;
static const unsigned char RENK = 0x0F;

void goruntu_temizle(void)
{
    for (int i = 0; i < GENISLIK * YUKSEKLIK; i++)
        vga_bellek[i] = (RENK << 8) | ' ';
    satir = 0;
    sutun = 0;
}

void goruntu_yaz(const char *metin)
{
    while (*metin)
    {
        if (*metin == '\n')
        {
            satir++;
            sutun = 0;
        }
        else
        {
            int konum = satir * GENISLIK + sutun;
            vga_bellek[konum] = (RENK << 8) | *metin;
            sutun++;
            if (sutun >= GENISLIK)
            {
                sutun = 0;
                satir++;
            }
        }
        metin++;
    }
}

void goruntu_yaz_hex(unsigned int deger)
{
    const char basamaklar[] = "0123456789ABCDEF";
    char arabellek[11];

    arabellek[0] = '0';
    arabellek[1] = 'x';
    arabellek[10] = '\0';

    for (int i = 0; i < 8; i++)
        arabellek[9 - i] = basamaklar[(deger >> (i * 4)) & 0xF];

    goruntu_yaz(arabellek);
}
