#ifndef PS2_H
#define PS2_H

#include <stdint.h>

void ps2_init(void);
int ps2_keyboard_read(void);
void ps2_mouse_read(int32_t *dx, int32_t *dy, int *left_click);

#endif
