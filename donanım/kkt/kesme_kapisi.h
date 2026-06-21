#ifndef KESME_KAPISI_H
#define KESME_KAPISI_H

struct idt_girdi {
    unsigned short alt;
    unsigned short secici;
    unsigned char sifir;
    unsigned char bayrak;
    unsigned short ust;
} __attribute__((packed));

struct idt_isaretci {
    unsigned short sinir;
    unsigned int taban;
} __attribute__((packed));

void idt_baslat(void);
void idt_kapi_ayarla(int numara, unsigned int konum);
void kesme_yoneticisi_kaydet(int numara, void (*isleyici)(void));

#endif
