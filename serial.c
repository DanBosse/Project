#include <stdint.h>

#define PORT 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// serial_init() is already provided by kernel_utils.c, so we omit it here.

void serial_write(char c) {
    outb(PORT, c);
}

void serial_print(const char *str) {
    while (*str) {
        serial_write(*str++);
    }
}

