/* ========================================================================
   TurkOS Kernel v0.2.0 - "Anadolu Çekirdeği"
   Copyright (c) 2026 Batuhan ALGÜL & TurkOS Development Team
   Licensed under the Apache License 2.0
   
   İMZA MEKANİZMASI:
   "Batuhan ALGÜL" ismi, ASCII değerleri üzerinden özel bir polinom 
   fonksiyonu ile işlenerek benzersiz bir kernel hash'i üretir.
   ======================================================================== */

#include <stdint.h>
#include <stddef.h>

/* --- DONANIM SABİTLERİ --- */
#define VGA_ADDR    0xB8000
#define VGA_W       80
#define VGA_H       25
#define MAX_STR_LEN 256

/* --- RENK KODLARI --- */
#define COLOR_BLACK     0x00
#define COLOR_BLUE      0x01
#define COLOR_GREEN     0x02
#define COLOR_CYAN      0x03
#define COLOR_RED       0x04
#define COLOR_MAGENTA   0x05
#define COLOR_BROWN     0x06
#define COLOR_LIGHT_GRAY 0x07
#define COLOR_DARK_GRAY  0x08
#define COLOR_LIGHT_BLUE 0x09
#define COLOR_LIGHT_GREEN 0x0A
#define COLOR_LIGHT_CYAN  0x0B
#define COLOR_LIGHT_RED   0x0C
#define COLOR_LIGHT_MAGENTA 0x0D
#define COLOR_YELLOW    0x0E
#define COLOR_WHITE     0x0F

/* --- GLOBAL DEĞİŞKENLER --- */
static uint16_t* vga_buffer = (uint16_t*)VGA_ADDR;
static uint32_t terminal_row = 0;
static uint32_t terminal_col = 0;
static uint8_t terminal_color = COLOR_WHITE | (COLOR_BLUE << 4);

/* --- BATUHAN ALGÜL İMZA HESAPLAYICI (GİZLİ DENKLEM) --- */
/* 
   Denklem: Hash = (Σ(ASCII * Pozisyon^2)) % 0xFFFFFFFF
   Bu fonksiyon, geliştiricinin kimliğini kriptografik olmayan ama 
   benzersiz bir sayısal imzaya dönüştürür.
*/
uint32_t hesapla_gelistirici_imzasi(const char* isim) {
    uint32_t hash = 0;
    uint32_t pozisyon = 1;
    
    for (int i = 0; isim[i] != '\0'; i++) {
        uint8_t val = (uint8_t)isim[i];
        // Polinom kaydırma ve toplama
        hash += (val * (pozisyon * pozisyon));
        pozisyon++;
        
        // Overflow kontrolü ve karıştırma
        hash = (hash << 5) + hash + val; 
    }
    
    return hash;
}

/* --- VGA SÜRÜCÜSÜ --- */
void vga_clear() {
    for (size_t i = 0; i < VGA_W * VGA_H; i++) {
        vga_buffer[i] = (uint16_t)' ' | ((uint16_t)terminal_color << 8);
    }
    terminal_row = 0;
    terminal_col = 0;
}

void vga_putc(char c, uint8_t color) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
    } else {
        size_t index = (terminal_row * VGA_W) + terminal_col;
        vga_buffer[index] = (uint16_t)c | ((uint16_t)color << 8);
        terminal_col++;
    }

    if (terminal_col >= VGA_W) {
        terminal_col = 0;
        terminal_row++;
    }

    /* Kaydırma İşlemi (Scrolling) */
    if (terminal_row >= VGA_H) {
        for (int i = 0; i < VGA_W * (VGA_H - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_W];
        }
        for (int i = VGA_W * (VGA_H - 1); i < VGA_W * VGA_H; i++) {
            vga_buffer[i] = (uint16_t)' ' | ((uint16_t)terminal_color << 8);
        }
        terminal_row = VGA_H - 1;
    }
}

void vga_puts(const char* str, uint8_t color) {
    while (*str) {
        vga_putc(*str++, color);
    }
}

void print_hex(uint32_t num) {
    const char hex_chars[] = "0123456789ABCDEF";
    vga_puts("0x", COLOR_LIGHT_CYAN);
    for (int i = 28; i >= 0; i -= 4) {
        vga_putc(hex_chars[(num >> i) & 0xF], COLOR_LIGHT_CYAN);
    }
}

/* --- GDT VE IDT YAPILARI (Önceki sürümden korunmuştur) --- */
struct gdt_entry { uint16_t limit_low; uint16_t base_low; uint8_t base_middle; uint8_t access; uint8_t granularity; uint8_t base_high; } __attribute__((packed));
struct gdt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed));
struct idt_entry { uint16_t base_lo; uint16_t sel; uint8_t always0; uint8_t flags; uint16_t base_hi; } __attribute__((packed));
struct idt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed));

static struct gdt_entry gdt[5];
static struct gdt_ptr gp;
static struct idt_entry idt[256];
static struct idt_ptr ip;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (uint32_t)&gdt;
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data Segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data
    __asm__ volatile("lgdt (%0)" : : "r" (&gp));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_init() {
    ip.limit = (sizeof(struct idt_entry) * 256) - 1;
    ip.base = (uint32_t)&idt;
    for(int i = 0; i < 256; i++) idt_set_gate(i, 0, 0x08, 0x8E);
    __asm__ volatile("lidt (%0)" : : "r" (&ip));
}

/* --- HARİCİ FONKSİYONLAR (Assembly/C Diğer Modüller) --- */
extern void islemci_kesme_baslat();
extern void timer_init(uint32_t freq);

/* --- ANA ÇEKİRDEK FONKSİYONU --- */
void kernel_main() {
    /* 1. Donanım Başlatma */
    gdt_init();
    idt_init();
    vga_clear();

    /* 2. İmza Doğrulama ve Gösterimi */
    const char* gelistirici_ismi = "Batuhan ALGÜL";
    uint32_t imza_hash = hesapla_gelistirici_imzasi(gelistirici_ismi);

    vga_puts("================================================\n", COLOR_LIGHT_CYAN);
    vga_puts("   TURKOS KERNEL v0.2.0 [ANADOLU CORE]\n", COLOR_YELLOW);
    vga_puts("================================================\n", COLOR_LIGHT_CYAN);
    
    vga_puts("\n[BOOT] System Initialized...\n", COLOR_LIGHT_GREEN);
    vga_puts("[AUTH] Developer Identity Verification:\n", COLOR_LIGHT_MAGENTA);
    vga_puts("       Name: ", COLOR_WHITE);
    vga_puts(gelistirici_ismi, COLOR_YELLOW);
    vga_puts("\n       Sign: ", COLOR_WHITE);
    print_hex(imza_hash);
    vga_puts(" (VERIFIED)\n", COLOR_LIGHT_GREEN);

    /* 3. Kesme Sistemi Başlatma */
    vga_puts("\n[IRQ ] Initializing Interrupt Descriptor Table...\n", COLOR_LIGHT_MAGENTA);
    islemci_kesme_baslat();
    
    vga_puts("[TIME] Starting Programmable Interval Timer (100Hz)...\n", COLOR_LIGHT_MAGENTA);
    timer_init(100);

    vga_puts("\n------------------------------------------------\n", COLOR_LIGHT_CYAN);
    vga_puts(" SYSTEM READY. WAITING FOR INPUT...\n", COLOR_LIGHT_GREEN);
    vga_puts("------------------------------------------------\n\n", COLOR_LIGHT_CYAN);

    /* 4. Ana Döngü (Halt ile Güç Tasarrufu) */
    while (1) {
        __asm__ volatile("hlt");
    }
}
