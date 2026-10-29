#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

#define PORT 0x3F8

void serial_init(void) {
    outb(PORT + 1, 0x00);    // Disable interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                         (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear with 14-byte threshold
    outb(PORT + 4, 0x0B);    // Enable IRQs, RTS/DSR set
}

int serial_received(void) {
    return inb(PORT + 5) & 1;
}

char serial_read(void) {
    while (serial_received() == 0);
    return inb(PORT);
}

int is_transmit_empty(void) {
    return inb(PORT + 5) & 0x20;
}

void serial_write(char a) {
    while (is_transmit_empty() == 0);
    outb(PORT, a);
}

void serial_print(const char* str) {
    while (*str) {
        serial_write(*str++);
    }
}
