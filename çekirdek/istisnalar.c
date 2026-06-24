/*
 * TurkOS - Istisna Yoneticisi / Exception Handler
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "istisnalar.h"
#include "panik.h"
#include "denetim/kesme_denetleyici.h"

static const char *istisna_adlari[20] = {
    "Sifira Bolme (Divide By Zero)",
    "Hata Ayiklama (Debug)",
    "Maskelenemez Kesme (NMI)",
    "Kesme Noktasi (Breakpoint)",
    "Tasma (Overflow)",
    "Sinir Disi (Bound Range)",
    "Gecersiz Komut (Invalid Opcode)",
    "Aygit Yok (Device Not Available)",
    "Cifte Hata (Double Fault)",
    "Bilinmeyen (Coprocessor Segment Overrun)",
    "Gecersiz TSS (Invalid TSS)",
    "Segment Yok (Segment Not Present)",
    "Yigin Hatasi (Stack-Segment Fault)",
    "Genel Koruma Hatasi (General Protection Fault)",
    "Sayfa Hatasi (Page Fault)",
    "Ayrilmis (Reserved)",
    "Kayan Nokta Hatasi (x87 FPU)",
    "Hizalama Hatasi (Alignment Check)",
    "Makine Kontrolu (Machine Check)",
    "SIMD Kayan Nokta Hatasi"
};

static inline unsigned int cr2_oku(void)
{
    unsigned int deger;
    __asm__ volatile ("mov %%cr2, %0" : "=r"(deger));
    return deger;
}

void istisna_isleyici(unsigned int numara, unsigned int kod)
{
    const char *isim = (numara < 20) ? istisna_adlari[numara] : "Bilinmeyen Istisna";

    kesme_denetleyici_istisna_bildir(numara, kod);

    if (numara == 14)
        panik(isim, cr2_oku());
    else
        panik(isim, kod);
}
