#ifndef HID_H
#define HID_H

#include <stdint.h>
#include <stdbool.h>

void hid_init(void);
uint8_t ps2_read_data(void);
void handle_keyboard_interrupt(uint8_t scancode);

#endif // HID_H
