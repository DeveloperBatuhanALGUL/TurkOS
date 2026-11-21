/*
 * TurkOS - Yigin Bekcisi / Stack Smashing Protector
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#include "panik.h"
#include "denetim/denetim_otobusu.h"
#include "../sürücüler/görüntü.h"

#define YIGIN_BOZULMASI_KODU 0x5C

__attribute__((noreturn)) void __stack_chk_fail(void)
{
    unsigned int donus = (unsigned int)__builtin_return_address(0);

    __asm__ volatile ("cli");
    goruntu_yaz("\n*** YIGIN BEKCISI: BOZULMA TESPIT EDILDI ***\n");
    denetim_olay_bildir(DENETIM_KAYNAK_BELLEK, DENETIM_SEVIYE_ALARM, YIGIN_BOZULMASI_KODU, donus);
    panik("Yigin Bozulmasi (Stack Smashing)", donus);

    for (;;)
        __asm__ volatile ("hlt");
}
