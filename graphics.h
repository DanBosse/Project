#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

typedef struct {
    uint32_t* base_address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} Framebuffer;

void graphics_init(void* limine_fb_request_response);
void put_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void clear_screen(uint32_t color);
void render_high_end_shader(int start_x, int start_y, int width, int height);

#endif
