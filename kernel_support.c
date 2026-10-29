#include <stdint.h>
#include <stdbool.h>

// --- x86 Port I/O Primitives ---
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

// --- Serial Port Driver (COM1: 0x3F8) ---
#define SERIAL_PORT 0x3F8

void serial_init(void) {
    outb(SERIAL_PORT + 1, 0x00); // Disable interrupts
    outb(SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00); // (hi byte)
    outb(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7); // Enable FIFO, clear with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int is_transmit_empty(void) {
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_write(char c) {
    while (is_transmit_empty() == 0);
    outb(SERIAL_PORT, c);
}

// --- IPC Subsystem Stub ---
void ipc_init(void) {
    // Initialize Inter-Process Communication tables
}

// --- Support & Wrapper Functions ---
static void support_print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_write(str[i]);
    }
}

void kernel_print(const char *str) {
    support_print(str);
}





void studiolens_render_telemetry_dashboard(void) {
    support_print("[StudioLens] Telemetry dashboard rendering...\r\n");
}

int serial_received(void) {
    return inb(SERIAL_PORT + 5) & 1;
}

char serial_read(void) {
    while (serial_received() == 0);
    return inb(SERIAL_PORT);
}
