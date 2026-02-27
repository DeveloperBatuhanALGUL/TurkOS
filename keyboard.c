/*
 * TurkOS PS/2 Keyboard Driver & Hardware I/O v0.1.2
 * Copyright (c) 2026 TurkOS Development Team
 * Licensed under the Apache License 2.0
 */

#include <stdint.h>

extern void vga_putc(char c, uint8_t color);

/* ==========================================
 * x86 DONANIM I/O (GİRİŞ/ÇIKIŞ) FONKSİYONLARI
 * ========================================== */

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

/* ==========================================
*  Developer Batuhan ALGÜL
* ========================================== */

const char kbd_TR[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_poll() {
    uint8_t status = inb(0x64);
    
    if (status & 0x01) { 
        uint8_t scancode = inb(0x60);
        
       
        if (!(scancode & 0x80)) { 
            char c = kbd_TR[scancode];
            if (c) {
                vga_putc(c, 0x07); 
            }
        }
    }
}
