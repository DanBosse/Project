#include "graphics_studio.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>
extern volatile struct limine_framebuffer_request framebuffer_request;
#define WIDTH 1024
#define HEIGHT 768
void graphics_studio_init(void) {
    serial_print("[GraphicsStudio] Initializing StudioLens Vector & Raster Design Canvas...\n");
}
static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}
void graphics_studio_render(void) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;
    // Canvas Studio Background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000F172A);
    // Left Tool Palette
    draw_rect(fb, WIDTH, HEIGHT, 50, 50, 80, HEIGHT - 100, 0x001E293B);
    // Central Artboard Canvas
    draw_rect(fb, WIDTH, HEIGHT, 160, 50, 780, HEIGHT - 100, 0x00FFFFFF);
    // Design Elements on Artboard
    draw_rect(fb, WIDTH, HEIGHT, 250, 150, 300, 200, 0x008B5CF6); // Primary vector layer
    draw_rect(fb, WIDTH, HEIGHT, 400, 300, 250, 250, 0x00EC4899); // Accent gradient layer
    serial_print("[GraphicsStudio] Design Canvas rendered successfully.\n");
}
