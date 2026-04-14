#include <stdint.h>

/*
 * TurkOS Kesme Tanımlama Tablosu (IDT) v0.1.0
 * Telif Hakkı (c) 2026 TurkOS Geliştirme Ekibi
 * Lisans: Apache License 2.0
 * Tanım: Donanım ve yazılım kesmelerini (IRQ) yöneten ana yapı.
 */

// Harici donanım ve assembly bağlantıları
extern void idt_load(); 
extern void outb(uint16_t kapi, uint8_t deger);


struct idt_entry {
    uint16_t base_low;   
    uint16_t sel;         
    uint8_t  always0;    
    uint8_t  flags;     
    uint16_t base_high;   
} __attribute__((packed));

// Tüm Hakları Açıtkır. :) 
struct idt_ptr {
    uint16_t limit;      
    uint32_t base;       
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr   idtp;


void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}


void pic_remap() {
    outb(0x20, 0x11); 
    outb(0xA0, 0x11); 
    outb(0x21, 0x20); 
    outb(0xA1, 0x28); 
    outb(0x21, 0x04); 
    outb(0xA1, 0x02);
    outb(0x21, 0x01); 
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  
    outb(0xA1, 0x0);
}


void idt_init() {
  
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

  
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

   
    pic_remap();

   
    idt_load();
}

// Batuhan ALGUL | Tüm Hakları Açıktır :)
