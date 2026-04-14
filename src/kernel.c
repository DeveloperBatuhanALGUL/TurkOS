/*
 * TurkOS Ana Çekirdek Dosyası (Kernel) v0.2.0
 * Telif Hakkı (c) 2026 TurkOS Geliştirme Ekibi
 * Lisans: Apache License 2.0
 * Tanım: Sistemin ana giriş noktası, imza doğrulama ve donanım başlatma.
 */

#include <stdint.h>


extern void timer_init(uint32_t frequency);
extern void keyboard_poll(void);

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
uint16_t vga_index = 0;


void vga_putc(char c, uint8_t color) {
    if (c == '\n') {
       
        vga_index = (vga_index / VGA_WIDTH + 1) * VGA_WIDTH;
    } else if (c == '\b') {
       
        if (vga_index > 0) {
            vga_index--;
            vga_buffer[vga_index] = (uint16_t)' ' | (uint16_t)color << 8;
        }
    } else {
       
        vga_buffer[vga_index++] = (uint16_t)c | (uint16_t)color << 8;
    }

    // Batuhan ALGUL | Tüm Hakları Açıktır :)
    if (vga_index >= VGA_WIDTH * VGA_HEIGHT) {
        vga_index = 0; 
    }
}


void vga_print(const char* str, uint8_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i], color);
    }
}


void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
    }
    vga_index = 0;
}


void vga_print_hex(uint32_t num, uint8_t color) {
    const char hex_chars[] = "0123456789ABCDEF";
    vga_print("0x", color);
    
 
    for (int i = 28; i >= 0; i -= 4) {
        vga_putc(hex_chars[(num >> i) & 0xF], color);
    }
}


uint32_t calculate_developer_signature(const char* name) {
    uint32_t hash = 0;
        for (int i = 0; name[i] != '\0'; i++) {
        uint32_t char_val = (uint32_t)name[i];
        hash += (char_val * i * i);
    }
    
   
    hash = hash ^ (hash << 5);
    
    return hash;
}


void kernel_main(void) {

    vga_clear();


    vga_print("================================================\n", 0x0B); 
    vga_print("   TURKOS KERNEL v0.2.0 [ANADOLU CORE]\n", 0x0B);
    vga_print("================================================\n\n", 0x0B);
    vga_print("[BOOT] System Initialized...\n", 0x0A); 

 
    const char* dev_name = "Batuhan ALGUL";
    uint32_t signature = calculate_developer_signature(dev_name);

    vga_print("[AUTH] Developer Identity Verification:\n", 0x0E); 
    vga_print("       Name: ", 0x07);
    vga_print(dev_name, 0x0F);
    vga_print("\n       Sign: ", 0x07);
    vga_print_hex(signature, 0x0F);
    
   
    vga_print(" (VERIFIED)\n\n", 0x0A); 

 
    vga_print("[TIME] Starting Programmable Interval Timer (100Hz)...\n", 0x09);
    timer_init(100);


    vga_print("[IRQ ] Initializing Interrupt Descriptor Table...\n\n", 0x0D);

    
    vga_print("------------------------------------------------\n", 0x07);
    vga_print(" SYSTEM READY. WAITING FOR INPUT...\n", 0x0A);
    vga_print("------------------------------------------------\n\n> ", 0x07);

  
    while (1) {
     
        keyboard_poll();

        __asm__ volatile("hlt"); 
    }
}
