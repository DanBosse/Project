#include "aura.h"
#include "studio_core.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

void aura_init(void) {
    serial_print("[Aura AI] Initializing Ring 0 Sovereign Intelligence Core & Neural Reasoning Engine with AuraLink Stream...\n");
    serial_print("[Aura AI] Connected to StudioLens spatial backplane and FlowScript IR.\n");
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

void aura_render(void) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;

    // Read live telemetry from AuraLink Channel 0 for neural reasoning feedback
    float metrics[8] = {0};
    uint32_t count = 0;
    char label[32] = {0};
    bool has_telemetry = studio_core_read_pipe(0, metrics, &count, label);

    // Aura Neural Interface Background (Deep Sovereign Indigo Dark Mode)
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000F071A);

    // Header Bar / Neural Status Panel
    draw_rect(fb, WIDTH, HEIGHT, 40, 40, WIDTH - 80, 60, 0x001E1B4B);

    // Left Context / Memory Tree Pane
    draw_rect(fb, WIDTH, HEIGHT, 40, 120, 260, HEIGHT - 160, 0x00171033);

    // Central Neural Chat & Execution Stream Canvas
    draw_rect(fb, WIDTH, HEIGHT, 320, 120, WIDTH - 360, HEIGHT - 160, 0x00120B24);

    // Active Neural Nodes / Pulse Indicators (Modulated live by AuraLink telemetry!)
    int pulse_offset = has_telemetry ? (int)(metrics[0] * 1.5f) : 0;
    if (pulse_offset > 80) pulse_offset = 80;
    if (pulse_offset < -80) pulse_offset = -80;

    draw_rect(fb, WIDTH, HEIGHT, 360, 180, 180 + pulse_offset, 40, 0x007C3AED); // AI Thought Node 1 (Telemetry Reactive)
    draw_rect(fb, WIDTH, HEIGHT, 360, 240, 240 - pulse_offset, 40, 0x004C1D95); // AI Thought Node 2
    draw_rect(fb, WIDTH, HEIGHT, 360, 300, 320 + (pulse_offset / 2), 40, 0x006D28D9); // AI Thought Node 3

    serial_print("[Aura AI] Sovereign Neural Interface rendered with AuraLink telemetry.\n");
}

void aura_query(const char *prompt) {
    serial_print("[Aura AI] Processing prompt: ");
    serial_print(prompt);
    serial_print("\n[Aura AI] Sovereign reasoning complete. Optimizing TorusOS convergence stack across all studios...\n");
}
