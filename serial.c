#include <stdint.h>

#define PORT 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void serial_print(const char *str) {
    while (*str) {
        outb(PORT, *str++);
    }
}
