/*
 * TurkOS - Goruntu Surucusu / VGA Text Display Driver
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "görüntü.h"

#define HATA_AYIKLAMA_PORTU 0xE9

static volatile unsigned short *vga_bellek = (unsigned short *)0xB8000;
static int satir = 0;
static int sutun = 0;
static const int GENISLIK = 80;
static const int YUKSEKLIK = 25;
static const unsigned char RENK = 0x0F;

static inline void hata_ayiklama_kanalina_yaz(char karakter)
{
    __asm__ volatile ("outb %0, %1" : : "a"(karakter), "Nd"((unsigned short)HATA_AYIKLAMA_PORTU));
}

static void satir_temizle(int hedef)
{
    for (int i = 0; i < GENISLIK; i++)
        vga_bellek[hedef * GENISLIK + i] = (RENK << 8) | ' ';
}

static void kaydir(void)
{
    for (int i = 0; i < GENISLIK * (YUKSEKLIK - 1); i++)
        vga_bellek[i] = vga_bellek[i + GENISLIK];
    satir_temizle(YUKSEKLIK - 1);
    satir = YUKSEKLIK - 1;
}

static void alt_satira_gec(void)
{
    sutun = 0;
    satir++;
    if (satir >= YUKSEKLIK)
        kaydir();
}

void goruntu_temizle(void)
{
    for (int i = 0; i < YUKSEKLIK; i++)
        satir_temizle(i);
    satir = 0;
    sutun = 0;
}

void goruntu_yaz(const char *metin)
{
    while (*metin)
    {
        hata_ayiklama_kanalina_yaz(*metin);
        if (*metin == '\n')
            alt_satira_gec();
        else
        {
            vga_bellek[satir * GENISLIK + sutun] = (RENK << 8) | (unsigned char)*metin;
            sutun++;
            if (sutun >= GENISLIK)
                alt_satira_gec();
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
