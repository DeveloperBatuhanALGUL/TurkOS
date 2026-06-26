#ifndef DENETIM_OTOBUSU_H
#define DENETIM_OTOBUSU_H

typedef enum {
    DENETIM_KAYNAK_GOREV,
    DENETIM_KAYNAK_BELLEK,
    DENETIM_KAYNAK_KESME,
    DENETIM_KAYNAK_SISTEM_CAGRISI,
    DENETIM_KAYNAK_DEVRIYE
} denetim_kaynak_t;

typedef enum {
    DENETIM_SEVIYE_BILGI,
    DENETIM_SEVIYE_SUPHELI,
    DENETIM_SEVIYE_ALARM
} denetim_seviye_t;

typedef struct {
    denetim_kaynak_t kaynak;
    denetim_seviye_t seviye;
    unsigned int kod;
    unsigned int detay;
} denetim_olay_t;

typedef void (*denetim_dinleyici_t)(const denetim_olay_t *olay);

void denetim_otobusu_baslat(void);
void denetim_olay_bildir(denetim_kaynak_t kaynak, denetim_seviye_t seviye, unsigned int kod, unsigned int detay);
void denetim_dinleyici_kaydet(denetim_dinleyici_t dinleyici);
void denetim_gunlugu_yazdir(void);
unsigned int denetim_alarm_sayisi(void);

#endif
