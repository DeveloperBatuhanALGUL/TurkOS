#include "gdt.h"

#define CIFT_HATA_YIGIN_BOYUTU 4096

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
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldt;
    unsigned short tuzak;
    unsigned short iopb;
} __attribute__((packed));

static struct gdt_girdi gdt[7];
static struct gdt_isaretci gdt_ptr;
static struct tss_girdi tss;
static struct tss_girdi cift_hata_tss;
static unsigned char cift_hata_yigini[CIFT_HATA_YIGIN_BOYUTU] __attribute__((aligned(16)));

extern void gdt_yukle(unsigned int);
extern void tss_yukle(unsigned short secici);
extern void cift_hata_gorevi(void);

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

void cift_hata_gorevi_hazirla(void)
{
    unsigned int cr3;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));
    cift_hata_tss.cr3 = cr3;
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

    cift_hata_tss.eip = (unsigned int)cift_hata_gorevi;
    cift_hata_tss.esp = (unsigned int)(cift_hata_yigini + CIFT_HATA_YIGIN_BOYUTU);
    cift_hata_tss.esp0 = cift_hata_tss.esp;
    cift_hata_tss.ss0 = 0x10;
    cift_hata_tss.eflags = 0x2;
    cift_hata_tss.cs = 0x08;
    cift_hata_tss.ds = 0x10;
    cift_hata_tss.es = 0x10;
    cift_hata_tss.fs = 0x10;
    cift_hata_tss.gs = 0x10;
    cift_hata_tss.ss = 0x10;
    cift_hata_tss.iopb = sizeof(cift_hata_tss);
    gdt_girdi_ayarla(6, (unsigned int)&cift_hata_tss, sizeof(cift_hata_tss) - 1, 0x89, 0x40);

    gdt_ptr.sinir = sizeof(gdt) - 1;
    gdt_ptr.taban = (unsigned int)&gdt;

    gdt_yukle((unsigned int)&gdt_ptr);
    tss_yukle(0x28);
}
