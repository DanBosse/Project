#include "audio_studio.h"
#include "studio_core.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

void audio_studio_init(void) {
    serial_print("[AudioStudio] Initializing StudioLens DAW & Multi-Track Mixing Console with AuraLink Stream...\n");
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

void audio_studio_render(void) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;

    // Read live physics telemetry from AuraLink Channel 0
    float metrics[8] = {0};
    uint32_t count = 0;
    char label[32] = {0};
    bool has_physics = studio_core_read_pipe(0, metrics, &count, label);

    // Console Background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x00090D16);

    // Master Meter Bridge
    draw_rect(fb, WIDTH, HEIGHT, 60, 50, WIDTH - 120, 120, 0x001E293B);

    // If physics telemetry is streaming, adjust fader 0 position dynamically based on spring position
    int fader_offset = 0;
    if (has_physics) {
        // Map spring position (-50 to 50 approx) to pixel offset
        fader_offset = (int)(metrics[0] * 2.0f);
        if (fader_offset > 80) fader_offset = 80;
        if (fader_offset < -80) fader_offset = -80;
    }

    // Channel Strips (Mixer faders)
    for (int i = 0; i < 6; i++) {
        draw_rect(fb, WIDTH, HEIGHT, 80 + (i * 150), 190, 120, 500, 0x001E293B);
        draw_rect(fb, WIDTH, HEIGHT, 120 + (i * 150), 240, 40, 320, 0x00334155); // Fader slot
        
        int knob_y = 400 + (i * 20);
        if (i == 0 && has_physics) {
            knob_y -= fader_offset; // Live reactive fader linked to physics spring!
        }
        draw_rect(fb, WIDTH, HEIGHT, 110 + (i * 150), knob_y, 60, 40, 0x00F59E0B); // Fader knob
    }

    serial_print("[AudioStudio] DAW Mixing Console rendered with AuraLink Stream.\n");
}
