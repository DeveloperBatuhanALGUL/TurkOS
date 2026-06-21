#ifndef BAGLANTI_H
#define BAGLANTI_H

static inline void port_byte_yaz(unsigned short port, unsigned char veri)
{
    __asm__ volatile ("outb %0, %1" : : "a"(veri), "Nd"(port));
}

static inline unsigned char port_byte_oku(unsigned short port)
{
    unsigned char sonuc;
    __asm__ volatile ("inb %1, %0" : "=a"(sonuc) : "Nd"(port));
    return sonuc;
}

#endif
