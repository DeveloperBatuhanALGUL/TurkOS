#include "gdt.h"

struct gdt_girdi {
    unsigned short sinir_alt;
    unsigned short taban_alt;
    unsigned char taban_orta;
    unsigned char erisim;
    unsigned char bayrak_sinir_ust;
    unsigned char taban_ust;
} __attribute__((packed));

struct gdt_isaretci {
    unsigned short sinir;
    unsigned int taban;
} __attribute__((packed));

struct tss_girdi {
    unsigned int onceki_gorev;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int alanlar[23];
    unsigned short iopb_sinir;
    unsigned short iopb;
} __attribute__((packed));

static struct gdt_girdi gdt[6];
static struct gdt_isaretci gdt_ptr;
static struct tss_girdi tss;

extern void gdt_yukle(unsigned int);
extern void tss_yukle(unsigned short secici);

static void gdt_girdi_ayarla(int dizin, unsigned int taban, unsigned int sinir,
                              unsigned char erisim, unsigned char bayrak)
{
    gdt[dizin].taban_alt = taban & 0xFFFF;
    gdt[dizin].taban_orta = (taban >> 16) & 0xFF;
    gdt[dizin].taban_ust = (taban >> 24) & 0xFF;
    gdt[dizin].sinir_alt = sinir & 0xFFFF;
    gdt[dizin].bayrak_sinir_ust = ((sinir >> 16) & 0x0F) | (bayrak & 0xF0);
    gdt[dizin].erisim = erisim;
}

void tss_yigin_ayarla(unsigned int esp0)
{
    tss.esp0 = esp0;
    tss.ss0 = 0x10;
}

void gdt_baslat(void)
{
    gdt_girdi_ayarla(0, 0, 0, 0, 0);
    gdt_girdi_ayarla(1, 0, 0xFFFFFFFF, 0x9A, 0xC0);
    gdt_girdi_ayarla(2, 0, 0xFFFFFFFF, 0x92, 0xC0);
    gdt_girdi_ayarla(3, 0, 0xFFFFFFFF, 0xFA, 0xC0);
    gdt_girdi_ayarla(4, 0, 0xFFFFFFFF, 0xF2, 0xC0);

    tss.esp0 = 0;
    tss.ss0 = 0x10;
    tss.iopb = sizeof(tss);

    gdt_girdi_ayarla(5, (unsigned int)&tss, sizeof(tss) - 1, 0x89, 0x40);

    gdt_ptr.sinir = sizeof(gdt) - 1;
    gdt_ptr.taban = (unsigned int)&gdt;

    gdt_yukle((unsigned int)&gdt_ptr);
    tss_yukle(0x28);
}
