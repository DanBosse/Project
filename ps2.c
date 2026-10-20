#include "ps2.h"
#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void ps2_wait_input(void) {
    while (inb(0x64) & 2);
}

void ps2_wait_output(void) {
    while (!(inb(0x64) & 1));
}

void ps2_init(void) {
    uint8_t status;

    outb(0x64, 0xAD);
    outb(0x64, 0xA7);

    inb(0x60);

    outb(0x64, 0x20);
    ps2_wait_output();
    status = (inb(0x60) & ~0x43);
    outb(0x64, 0x60);
    ps2_wait_input();
    outb(0x60, status);

    outb(0x64, 0xA8);
    outb(0x64, 0xAE);
    outb(0x64, 0xA5);

    outb(0x64, 0xD4);
    ps2_wait_input();
    outb(0x60, 0xF6);
    ps2_wait_output();
    inb(0x60);

    outb(0x64, 0xD4);
    ps2_wait_input();
    outb(0x60, 0xF4);
    ps2_wait_output();
    inb(0x60);
}

int ps2_keyboard_read(void) {
    uint8_t status = inb(0x64);
    // Bit 0 = Output buffer full, Bit 5 = 0 for keyboard data
    if ((status & 1) && !(status & 0x20)) {
        return inb(0x60);
    }
    return -1;
}

static uint8_t mouse_cycle = 0;
static int8_t mouse_bytes[3];

void ps2_mouse_read(int32_t *dx, int32_t *dy, int *left_click) {
    uint8_t status = inb(0x64);
    // Bit 0 = Output buffer full, Bit 5 = 1 for mouse data
    if ((status & 1) && (status & 0x20)) {
        uint8_t data = inb(0x60);

        if (mouse_cycle == 0) {
            if (data & 0x08) {
                mouse_bytes[0] = data;
                mouse_cycle++;
            }
        } else if (mouse_cycle == 1) {
            mouse_bytes[1] = data;
            mouse_cycle++;
        } else if (mouse_cycle == 2) {
            mouse_bytes[2] = data;
            mouse_cycle = 0;

            *dx = (int32_t)mouse_bytes[1] - (((mouse_bytes[0] & 0x10) << 4));
            *dy = -((int32_t)mouse_bytes[2] - (((mouse_bytes[0] & 0x20) << 3)));
            *left_click = mouse_bytes[0] & 0x01;
        }
    }
}
