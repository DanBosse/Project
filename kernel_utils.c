#include <stdint.h>
#include <stddef.h>
#include "kernel_utils.h"

// VGA Text Mode Buffer Pointer
volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
static int terminal_row = 0;
static int terminal_col = 0;
static const uint8_t terminal_color = 0x0F; // White on black

// External outb from ports.c
extern void outb(uint16_t port, uint8_t val);

void kernel_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= 25) terminal_row = 24;
        return;
    }
    
    const size_t index = terminal_row * 80 + terminal_col;
    vga_buffer[index] = ((uint16_t)terminal_color << 8) | (uint16_t)c;
    terminal_col++;
    if (terminal_col >= 80) {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= 25) terminal_row = 24;
    }
}

void kernel_print(const char *str) {
    while (*str) {
        kernel_putchar(*str);
        // Also send to serial port 0x3F8 for debugging
        outb(0x3F8, *str);
        str++;
    }
}
