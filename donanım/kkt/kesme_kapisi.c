#include "kesme_kapisi.h"
#include "../../ortak/bağlantı.h"

static struct idt_girdi idt[256];
static struct idt_isaretci idt_ptr;
static void (*kesme_isleyicileri[256])(void);

extern void idt_yukle(unsigned int);
extern void irq0_giris(void);
extern void irq1_giris(void);
extern void int80_giris(void);

#define ISTISNA_DISARI(n) extern void isr##n##_giris(void);
ISTISNA_DISARI(0) ISTISNA_DISARI(1) ISTISNA_DISARI(2) ISTISNA_DISARI(3)
ISTISNA_DISARI(4) ISTISNA_DISARI(5) ISTISNA_DISARI(6) ISTISNA_DISARI(7)
ISTISNA_DISARI(8) ISTISNA_DISARI(9) ISTISNA_DISARI(10) ISTISNA_DISARI(11)
ISTISNA_DISARI(12) ISTISNA_DISARI(13) ISTISNA_DISARI(14) ISTISNA_DISARI(15)
ISTISNA_DISARI(16) ISTISNA_DISARI(17) ISTISNA_DISARI(18) ISTISNA_DISARI(19)
ISTISNA_DISARI(20) ISTISNA_DISARI(21) ISTISNA_DISARI(22) ISTISNA_DISARI(23)
ISTISNA_DISARI(24) ISTISNA_DISARI(25) ISTISNA_DISARI(26) ISTISNA_DISARI(27)
ISTISNA_DISARI(28) ISTISNA_DISARI(29) ISTISNA_DISARI(30) ISTISNA_DISARI(31)

static void (*istisna_girisleri[32])(void) = {
    isr0_giris, isr1_giris, isr2_giris, isr3_giris,
    isr4_giris, isr5_giris, isr6_giris, isr7_giris,
    isr8_giris, isr9_giris, isr10_giris, isr11_giris,
    isr12_giris, isr13_giris, isr14_giris, isr15_giris,
    isr16_giris, isr17_giris, isr18_giris, isr19_giris,
    isr20_giris, isr21_giris, isr22_giris, isr23_giris,
    isr24_giris, isr25_giris, isr26_giris, isr27_giris,
    isr28_giris, isr29_giris, isr30_giris, isr31_giris
};

void idt_kapi_ayarla(int numara, unsigned int konum)
{
    idt[numara].alt = konum & 0xFFFF;
    idt[numara].secici = 0x08;
    idt[numara].sifir = 0;
    idt[numara].bayrak = 0x8E;
    idt[numara].ust = (konum >> 16) & 0xFFFF;
}

void idt_kapi_ayarla_kullanici(int numara, unsigned int konum)
{
    idt[numara].alt = konum & 0xFFFF;
    idt[numara].secici = 0x08;
    idt[numara].sifir = 0;
    idt[numara].bayrak = 0xEE;
    idt[numara].ust = (konum >> 16) & 0xFFFF;
}

void kesme_yoneticisi_kaydet(int numara, void (*isleyici)(void))
{
    kesme_isleyicileri[numara] = isleyici;
}

static void pic_yenidenharita(void)
{
    port_byte_yaz(0x20, 0x11);
    port_byte_yaz(0xA0, 0x11);
    port_byte_yaz(0x21, 0x20);
    port_byte_yaz(0xA1, 0x28);
    port_byte_yaz(0x21, 0x04);
    port_byte_yaz(0xA1, 0x02);
    port_byte_yaz(0x21, 0x01);
    port_byte_yaz(0xA1, 0x01);
    port_byte_yaz(0x21, 0x0);
    port_byte_yaz(0xA1, 0x0);
}

void kesme_gonder(int numara)
{
    if (kesme_isleyicileri[numara])
        kesme_isleyicileri[numara]();

    if (numara >= 40)
        port_byte_yaz(0xA0, 0x20);
    port_byte_yaz(0x20, 0x20);
}

void idt_baslat(void)
{
    idt_ptr.sinir = sizeof(idt) - 1;
    idt_ptr.taban = (unsigned int)&idt;

    for (int i = 0; i < 256; i++)
        idt_kapi_ayarla(i, 0);

    for (int i = 0; i < 32; i++)
        idt_kapi_ayarla(i, (unsigned int)istisna_girisleri[i]);

    idt_kapi_ayarla(32, (unsigned int)irq0_giris);
    idt_kapi_ayarla(33, (unsigned int)irq1_giris);
    idt_kapi_ayarla_kullanici(0x80, (unsigned int)int80_giris);

    pic_yenidenharita();
    idt_yukle((unsigned int)&idt_ptr);
}
