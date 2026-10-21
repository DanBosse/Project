#include "video_studio.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>
extern volatile struct limine_framebuffer_request framebuffer_request;
#define WIDTH 1024
#define HEIGHT 768
void video_studio_init(void) {
    serial_print("[VideoStudio] Initializing StudioLens NLE Timeline & Monitor...\n");
}
static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}
void video_studio_render(void) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;
    // Canvas background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000F172A);
    // Program Monitor Preview Window
    draw_rect(fb, WIDTH, HEIGHT, 60, 50, 560, 320, 0x001E293B);
    // Timeline Tracks
    draw_rect(fb, WIDTH, HEIGHT, 60, 400, WIDTH - 120, 310, 0x001E293B);
    draw_rect(fb, WIDTH, HEIGHT, 80, 430, WIDTH - 160, 60, 0x00038BDF8); // V1 Track clip
    draw_rect(fb, WIDTH, HEIGHT, 80, 510, WIDTH - 220, 60, 0x0010B981); // A1 Audio waveform track
    serial_print("[VideoStudio] NLE Timeline & Preview rendered successfully.\n");
}
