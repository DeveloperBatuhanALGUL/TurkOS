/* ========================================================================
   TurkOS - KESME YÖNETİCİSİ (INTERRUPT MANAGER)
   Tanım: Donanım kesmelerini işleyen merkezi C fonksiyonu.
   Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
   Tarih: 2026
   ======================================================================== */

#include <stdint.h>
#include "include/islemci.h"

/* Dışarıdan alınan fonksiyonlar */
extern void vga_putc(char c, uint8_t color);
extern void keyboard_poll();
extern void timer_interrupt_handler();
extern void outb(uint16_t port, uint8_t val);

/* PIC (Programmable Interrupt Controller) Komutları */
#define PIC_KOMUT_1 0x20
#define PIC_KOMUT_2 0xA0
#define PIC_EOI     0x20 /* End of Interrupt */

volatile uint32_t toplam_sistem_tik_sayisi = 0;

/* C Tarafından Çağrılan Kesme İşleyici */
void c_islemci_kesme_isleyici(uint32_t kesme_no) {
    /* 1. Timer Kesmesi (IRQ 0 -> ISR 32) */
    if (kesme_no == 32) {
        toplam_sistem_tik_sayisi++;
        
        /* Ekrana basit bir animasyon veya tick göstergesi */
        if (toplam_sistem_tik_sayisi % 50 == 0) {
             /* Her 50 tick'te bir imleci hareket ettir veya bir şey yaz */
             /* Not: Burası çok sık çalıştığı için ağır işlem koyma */
        }
        
        /* PIC'e işin bittiğini bildir (EOI) */
        outb(PIC_KOMUT_1, PIC_EOI);
    }
    
    /* 2. Klavye Kesmesi (IRQ 1 -> ISR 33) */
    else if (kesme_no == 33) {
        keyboard_poll(); /* Klavyeden veri oku ve ekrana bas */
        
        /* PIC'e işin bittiğini bildir (EOI) */
        outb(PIC_KOMUT_1, PIC_EOI);
    }
    
    /* Diğer Kesmeler (Hata vb.) */
    else {
        /* Bilinmeyen kesme için log veya hata mesajı */
    }
}

/* Kesme Tablosunu Aktif Etme */
void islemci_kesme_baslat() {
    /* IDT zaten kernel_main'de init edildi, burada sadece maskeleri açıyoruz */
    
    /* PIC Remapping (Varsayılan olarak 0-7 ve 8-15 yerine 32-47 aralığına alıyoruz) */
    outb(0x20, 0x11); /* ICW1: Init */
    outb(0xA0, 0x11);
    
    outb(0x21, 0x20); /* ICW2: IRQ 0-7 -> ISR 32-39 */
    outb(0xA1, 0x28); /* ICW2: IRQ 8-15 -> ISR 40-47 */
    
    outb(0x21, 0x04); /* ICW3: Master PIC has slave on IR2 */
    outb(0xA1, 0x02);
    
    outb(0x21, 0x01); /* ICW4: 8086 mode */
    outb(0xA1, 0x01);
    
    outb(0x21, 0x00); /* OCW1: Mask all interrupts initially */
    outb(0xA1, 0x00);
    
    /* Sadece Timer (IRQ0) ve Klavye (IRQ1) izin ver */
    outb(0x21, 0xFC); /* 1111 1100 -> IRQ 0 ve 1 açık */
    outb(0xA1, 0xFF); /* Slave tamamen kapalı */
}
