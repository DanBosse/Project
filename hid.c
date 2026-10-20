#include "torus.h"
#include "hid.h"

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64

extern void spatial_compositor_handle_char(char c);
extern void spatial_compositor_update_scancode(uint8_t scancode);

static int extended_code = 0;
static int break_code = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void hid_init(void) {
    while (inb(PS2_STATUS_PORT) & 1) {
        inb(PS2_DATA_PORT);
    }
}

// Comprehensive Scancode Set 2 Mapping (Letters, Numbers, Punctuation)
static const char scancode_set2_ascii[128] = {
    [0x1C] = 'a', [0x32] = 'b', [0x21] = 'c', [0x23] = 'd', [0x24] = 'e',
    [0x2B] = 'f', [0x34] = 'g', [0x33] = 'h', [0x43] = 'i', [0x3B] = 'j',
    [0x42] = 'k', [0x4B] = 'l', [0x3A] = 'm', [0x31] = 'n', [0x44] = 'o',
    [0x4D] = 'p', [0x15] = 'q', [0x2D] = 'r', [0x1B] = 's', [0x2C] = 't',
    [0x3C] = 'u', [0x2A] = 'v', [0x1D] = 'w', [0x22] = 'x', [0x35] = 'y',
    [0x1A] = 'z',
    [0x45] = '0', [0x16] = '1', [0x1E] = '2', [0x26] = '3', [0x25] = '4',
    [0x2E] = '5', [0x36] = '6', [0x3D] = '7', [0x3E] = '8', [0x46] = '9',
    [0x0E] = '`', [0x4E] = '-', [0x55] = '=', [0x54] = '[', [0x5B] = ']',
    [0x5D] = '\\', [0x4C] = ';', [0x52] = '\'', [0x41] = ',', [0x49] = '.',
    [0x4A] = '/', [0x29] = ' ', [0x5A] = '\n', [0x66] = '\b', [0x0D] = '\t'
};

void handle_keyboard_interrupt(uint8_t scancode) {
    spatial_compositor_update_scancode(scancode);

    if (scancode == 0xE0) {
        extended_code = 1;
        return;
    }

    if (scancode == 0xF0) {
        break_code = 1;
        return;
    }

    if (break_code) {
        break_code = 0;
        extended_code = 0;
        return;
    }

    if (extended_code) {
        extended_code = 0;
        return;
    }

    if (scancode < 128) {
        char ascii = scancode_set2_ascii[scancode];
        if (ascii != 0) {
            spatial_compositor_handle_char(ascii);
        }
    }
}
