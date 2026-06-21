#include "kesme_kapisi.h"
#include "../../ortak/bağlantı.h"

static struct idt_girdi idt[256];
static struct idt_isaretci idt_ptr;
static void (*kesme_isleyicileri[256])(void);

extern void idt_yukle(unsigned int);
extern void irq1_giris(void);

void idt_kapi_ayarla(int numara, unsigned int konum)
{
    idt[numara].alt = konum & 0xFFFF;
    idt[numara].secici = 0x08;
    idt[numara].sifir = 0;
    idt[numara].bayrak = 0x8E;
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

    idt_kapi_ayarla(33, (unsigned int)irq1_giris);

    pic_yenidenharita();
    idt_yukle((unsigned int)&idt_ptr);
}
