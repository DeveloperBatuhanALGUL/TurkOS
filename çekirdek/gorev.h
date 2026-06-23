#ifndef GOREV_H
#define GOREV_H

typedef struct gorev {
    unsigned int esp;
    struct gorev *sonraki;
} gorev_t;

gorev_t *gorev_olustur(void (*fonksiyon)(void), unsigned int *yigin, unsigned int boyut);
void gorev_zincirle(gorev_t *birinci, gorev_t *ikinci);
void gorev_baslat(gorev_t *ilk_gorev);
void gorev_yield(void);

#endif
