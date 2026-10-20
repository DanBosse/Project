#include <stdint.h>

// External CLI character handler
extern void cli_handle_char(char c);

// Basic PS/2 scancode to ASCII mapping table (Set 1 US layout example)
static char ps2_ascii_table[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// PS/2 Keyboard interrupt/scancode processing entry point
void ps2_keyboard_handler(uint8_t scancode) {
    // Ignore key release scancodes (high bit set)
    if (scancode & 0x80) {
        return;
    }

    if (scancode < 128) {
        char ascii = ps2_ascii_table[scancode];
        if (ascii != 0) {
            cli_handle_char(ascii);
        }
    }
}
