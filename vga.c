#include <stdint.h>

volatile uint16_t* vga_buffer = (uint16_t*) 0xB8000;
const uint8_t vga_color = 0x0F;

void vga_print_string(const char* str, int* row, int col) {
    int index = (*row * 80) + col;
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == '\n') {
            (*row)++;
            index = (*row * 80) + col;
            continue;
        }
        vga_buffer[index++] = (vga_color << 8) | str[i];
    }
}
