/*
 * TurkOS - Cekirdek Panik Modulu / Kernel Panic Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#ifndef PANIK_H
#define PANIK_H

void panik(const char *mesaj, unsigned int kod) __attribute__((noreturn));

#endif
