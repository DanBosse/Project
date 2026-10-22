#include "video_studio.h"
#include "studio_core.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

void video_studio_init(void) {
    serial_print("[VideoStudio] Initializing StudioLens NLE Timeline & Monitor with AuraLink Stream...\n");
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

    // Read live physics telemetry from AuraLink Channel 0 for automated keyframing
    float metrics[8] = {0};
    uint32_t count = 0;
    char label[32] = {0};
    bool has_physics = studio_core_read_pipe(0, metrics, &count, label);

    // Canvas background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000F172A);

    // Program Monitor Preview Window
    draw_rect(fb, WIDTH, HEIGHT, 60, 50, 560, 320, 0x001E293B);

    // If physics data is streaming, draw a live animated telemetry curve inside the preview window!
    if (has_physics) {
        int cx = 60 + 280;
        int cy = 50 + 160;
        int offset_x = (int)(metrics[0] * 2.0f);
        draw_rect(fb, WIDTH, HEIGHT, cx + offset_x - 10, cy - 10, 20, 20, 0x0038BDF8);
    }

    // Timeline Tracks
    draw_rect(fb, WIDTH, HEIGHT, 60, 400, WIDTH - 120, 310, 0x001E293B);
    
    // V1 Track clip width/position modulated live by physics telemetry!
    int v1_width = WIDTH - 160;
    if (has_physics) {
        int mod = (int)(metrics[1] * 1.5f);
        if (v1_width + mod > 100 && v1_width + mod < (int)(WIDTH - 100)) {
            v1_width += mod;
        }
    }
    draw_rect(fb, WIDTH, HEIGHT, 80, 430, v1_width, 60, 0x0038BDF8); // V1 Track clip (Keyframed by AuraLink!)
    draw_rect(fb, WIDTH, HEIGHT, 80, 510, WIDTH - 220, 60, 0x0010B981); // A1 Audio waveform track

    serial_print("[VideoStudio] NLE Timeline & Preview rendered with AuraLink Stream.\n");
}
