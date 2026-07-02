/*
 * TurkOS - Gorev Denetleyici Modulu / Task Supervisor Module
 * Telif Hakki (c) 2026 Batuhan ALGUL - Tum haklari saklidir.
 * Copyright (c) 2026 Batuhan ALGUL - All rights reserved.
 */
#ifndef GOREV_DENETLEYICI_H
#define GOREV_DENETLEYICI_H

#include "../gorev.h"

void gorev_denetleyici_baslat(void);
void gorev_denetleyici_gecis_bildir(gorev_t *eski, gorev_t *yeni);
void gorev_denetleyici_supheli_bildir(gorev_t *g);
unsigned int gorev_denetleyici_aktif_sayisi(void);
unsigned int gorev_denetleyici_karantina_sayisi(void);

#endif
