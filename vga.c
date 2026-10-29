

#include <stdint.h>
#include <stddef.h>

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}


static volatile uint16_t* const VGA_BUFFER = (uint16_t*) 0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
static int terminal_row = 0;
static int terminal_column = 0;
static uint8_t terminal_color = 0x07;

void vga_init(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
    terminal_row = 0;
    terminal_column = 0;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    VGA_BUFFER[index] = ((uint16_t)terminal_color << 8) | c;
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void vga_print(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}
