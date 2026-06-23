/*
 * TurkOS - Cekirdek Panik Modulu / Kernel Panic Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "panik.h"
#include "../sürücüler/görüntü.h"

void panik(const char *mesaj, unsigned int kod)
{
    __asm__ volatile ("cli");

    goruntu_yaz("\n--- TURKOS CEKIRDEK PANIGI ---\n");
    goruntu_yaz(mesaj);
    goruntu_yaz("\nKod: ");
    goruntu_yaz_hex(kod);
    goruntu_yaz("\nSistem durduruldu.\n");

    for (;;)
        __asm__ volatile ("hlt");
}
