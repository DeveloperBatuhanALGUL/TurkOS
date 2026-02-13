/*
 * TurkOS Kernel v0.1.2
 * Copyright (c) 2026 TurkOS Development Team
 * Licensed under the Apache License 2.0
 */

#include <stdint.h>

#define VGA_ADDR 0xB8000
#define VGA_W 80
#define VGA_H 25

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct gdt_entry gdt[5];
static struct gdt_ptr gp;
static struct idt_entry idt[256];
static struct idt_ptr ip;

static uint16_t* vga = (uint16_t*)VGA_ADDR;
static uint32_t x = 0;
static uint32_t y = 0;

void vga_putc(char c, uint8_t color) {
    if (c == '\n') {
        x = 0;
        y++;
    } else {
        vga[y * VGA_W + x] = (uint16_t)c | (uint16_t)color << 8;
        x++;
    }

    if (x >= VGA_W) {
        x = 0;
        y++;
    }
    
    if (y >= VGA_H) {
        for (int i = 0; i < VGA_W * (VGA_H - 1); i++) {
            vga[i] = vga[i + VGA_W];
        }
        for (int i = VGA_W * (VGA_H - 1); i < VGA_W * VGA_H; i++) {
            vga[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
        }
        y = VGA_H - 1;
    }
}

void vga_puts(const char* str, uint8_t color) {
    while (*str) vga_putc(*str++, color);
}

void vga_clear() {
    for (int i = 0; i < VGA_W * VGA_H; i++) {
        vga[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
    }
    x = 0;
    y = 0;
}

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access      = access;
}

void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 

    __asm__ volatile("lgdt (%0)" : : "r" (&gp));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

void idt_init() {
    ip.limit = (sizeof(struct idt_entry) * 256) - 1;
    ip.base  = (uint32_t)&idt;

    for(int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }

    __asm__ volatile("lidt (%0)" : : "r" (&ip));
}

void kernel_main() {
    gdt_init();
    idt_init();
    vga_clear();

    vga_puts("TurkOS v0.1.2 Deployment\n", 0x0C);
    vga_puts("System: x86 Protected Mode Active\n", 0x0A);
    vga_puts("Memory: GDT Segmentation Success\n", 0x0A);
    vga_puts("Interrupts: IDT Gates Ready\n", 0x0A);
    
    vga_puts("\nWaiting for hardware signals...\n", 0x0F);

    while (1) {
        __asm__ volatile("hlt");
    }
}
