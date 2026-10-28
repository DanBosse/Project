#include "graphics.h"
#include "phasecore.h"

static uint32_t dummy_fb[1024 * 768];
static uint32_t fb_width = 1024;
static uint32_t fb_height = 768;
static uint32_t fb_pitch = 1024 * 4;

void graphics_init(uint32_t *fb_addr, uint32_t width, uint32_t height, uint32_t pitch) {
    if (fb_addr) {
        // use real framebuffer if provided by bootloader
    } else {
        // fallback to internal memory buffer for simulation
    }
    fb_width = width ? width : 1024;
    fb_height = height ? height : 768;
    fb_pitch = pitch ? pitch : fb_width * 4;
}

void graphics_clear(uint32_t color) {
    for (uint32_t i = 0; i < fb_width * fb_height; i++) {
        dummy_fb[i] = color;
    }
}

void graphics_draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x < fb_width && y < fb_height) {
        dummy_fb[y * fb_width + x] = color;
    }
}

void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    for (uint32_t dy = 0; dy < height; dy++) {
        for (uint32_t dx = 0; dx < width; dx++) {
            graphics_draw_pixel(x + dx, y + dy, color);
        }
    }
}

void graphics_render_test_pattern(void) {
    graphics_clear(0x000F2050);
    graphics_draw_rect(0, 0, fb_width, 40, 0x001F4080);
    graphics_draw_rect(50, 80, 400, 250, 0x00203060);
    phasecore_dispatch(TENSOR_MAP, 0x05, 0x00);
}
