#ifndef GOREV_H
#define GOREV_H

typedef enum {
    GOREV_DURUM_CALISIYOR,
    GOREV_DURUM_SUPHELI,
    GOREV_DURUM_KARANTINA,
    GOREV_DURUM_SONLANDIRILDI
} gorev_durum_t;

typedef struct gorev {
    unsigned int esp;
    struct gorev *sonraki;
    gorev_durum_t durum;
    unsigned int id;
} gorev_t;

gorev_t *gorev_olustur(void (*fonksiyon)(void), unsigned int *yigin, unsigned int boyut);
void gorev_zincirle(gorev_t *birinci, gorev_t *ikinci);
void gorev_baslat(gorev_t *ilk_gorev);
void gorev_yield(void);
void gorev_karantinaya_al(gorev_t *g);
void gorev_sonlandir(gorev_t *g);
gorev_t *gorev_su_anki(void);

#endif
