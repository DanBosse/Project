#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>

void graphics_init(uint32_t *fb_addr, uint32_t width, uint32_t height, uint32_t pitch);
void graphics_clear(uint32_t color);
void graphics_draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void graphics_render_test_pattern(void);

#endif
