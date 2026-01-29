// TurkOS Kernel v0.1
// Batuhan ALGUL, 2026
// Apache License 2.0

typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
static uint32_t cursor_row = 0;
static uint32_t cursor_col = 0;

enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_YELLOW = 14,
    COLOR_WHITE = 15
};

static inline uint8_t vga_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void terminal_clear(void) {
    uint8_t color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }
    cursor_row = 0;
    cursor_col = 0;
}

void terminal_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            cursor_row = 0;
        }
        return;
    }
    
    uint32_t index = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[index] = vga_entry(c, color);
    
    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            cursor_row = 0;
        }
    }
}

void terminal_print(const char* str, uint8_t color) {
    while (*str) {
        terminal_putchar(*str++, color);
    }
}

void kernel_main(void) {
    terminal_clear();
    
    uint8_t title_color = vga_color(COLOR_LIGHT_RED, COLOR_BLACK);
    terminal_print("================================================================================\n", title_color);
    terminal_print("                              TurkOS v0.1 Alpha                                \n", title_color);
    terminal_print("================================================================================\n\n", title_color);
    
    uint8_t success_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    terminal_print("[OK] Kernel initialized\n", success_color);
    terminal_print("[OK] VGA text mode active\n", success_color);
    terminal_print("[OK] Memory ready\n", success_color);
    terminal_print("[OK] System services loaded\n\n", success_color);
    
    uint8_t info_color = vga_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("System Information:\n", info_color);
    
    uint8_t normal_color = vga_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("  Version:     0.1 Alpha\n", normal_color);
    terminal_print("  Architecture: x86 (32-bit)\n", normal_color);
    terminal_print("  Build Date:   ", normal_color);
    terminal_print(__DATE__, normal_color);
    terminal_print("\n", normal_color);
    terminal_print("  Developer:    Batuhan ALGUL\n", normal_color);
    terminal_print("  License:      Apache 2.0\n\n", normal_color);
    
    uint8_t copyright_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_print("Turkish Open Source Operating System\n", copyright_color);
    terminal_print("Copyright (c) 2026 Batuhan ALGUL. All rights reserved.\n\n", copyright_color);
    
    uint8_t status_color = vga_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_print(">>> Kernel running. System halted.\n", status_color);
    terminal_print(">>> Close virtual machine to exit.\n", status_color);
    
    while (1) {
        __asm__ volatile ("hlt");
    }
}