#ifndef SPATIAL_COMPOSITOR_H
#define SPATIAL_COMPOSITOR_H

#include <stdint.h>
#include "studiolens.h"

typedef struct {
    char title[32];
    uint32_t x, y, w, h;
    uint32_t z_index;
    int is_active;
    int is_minimized;
} SpatialWindow;

void spatial_compositor_init(void);
void spatial_compositor_render(uint32_t flags);
void spatial_compositor_update_scancode(uint8_t scancode);
void spatial_compositor_handle_char(char c);

int spatial_compositor_create_window(const char* title, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void spatial_compositor_focus_window(int index);

void spatial_compositor_mouse_move(int32_t x, int32_t y);
void spatial_compositor_mouse_click(int32_t x, int32_t y, uint8_t button_state);

#endif
