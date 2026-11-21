#ifndef GDT_H
#define GDT_H

void gdt_baslat(void);
void tss_yigin_ayarla(unsigned int esp0);
void cift_hata_gorevi_hazirla(void);

#endif
