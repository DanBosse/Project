
#include <stdint.h>
#include <stddef.h>
#include "graphics_studio.h"
#include "topological.h"
#include "torus.h"
#include "limine/limine.h"

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

void graphics_studio_init(void) {
    serial_print("[GraphicsStudio] Initializing StudioLens Vector & Raster Design Canvas...\n");
    topological_init();
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

static void draw_border(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    draw_rect(fb, w, h, rx, ry, rw, 2, color);
    draw_rect(fb, w, h, rx, ry + rh - 2, rw, 2, color);
    draw_rect(fb, w, h, rx, ry, 2, rh, color);
    draw_rect(fb, w, h, rx + rw - 2, ry, 2, rh, color);
}

void graphics_studio_render(void) {
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count == 0) {
        serial_print("[GraphicsStudio ERROR] No framebuffer available!\n");
        return;
    }

    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t *)fb_info->address;
    uint32_t w = WIDTH;
    uint32_t h = HEIGHT;

    // 1. Dark modern workstation theme background
    draw_rect(fb, w, h, 0, 0, w, h, 0x000F172A);

    // 2. Main Topological Manifold Canvas Viewport (Left)
    draw_rect(fb, w, h, 24, 24, 600, 600, 0x001E293B);
    draw_border(fb, w, h, 24, 24, 600, 600, 0x00334155);
    
    // Render topological frame pass
    topological_rasterize_frame(fb, w, h, fb_info->pitch, 42);

    // 3. Telemetry & Performance Dashboard (Right)
    int dash_x = 648;
    int dash_w = 350;
    draw_rect(fb, w, h, dash_x, 24, dash_w, 600, 0x001E293B);
    draw_border(fb, w, h, dash_x, 24, dash_w, 600, 0x00334155);

    // GPU Utilization Bar
    draw_rect(fb, w, h, dash_x + 24, 124, dash_w - 48, 20, 0x00334155);
    draw_rect(fb, w, h, dash_x + 24, 124, (dash_w - 48) * 92 / 100, 20, 0x0010B981);

    // Pipeline Latency Graph Track
    draw_rect(fb, w, h, dash_x + 24, 198, dash_w - 48, 80, 0x000F172A);
    draw_border(fb, w, h, dash_x + 24, 198, dash_w - 48, 80, 0x00334155);

    for(int x = 0; x < dash_w - 60; x += 6) {
        int bar_h = 15 + ((x * 13) % 45);
        draw_rect(fb, w, h, dash_x + 30 + x, 278 - bar_h, 4, bar_h, 0x0038BDF8);
    }

    // Subsystem Convergence Status Box
    draw_rect(fb, w, h, dash_x + 24, 334, dash_w - 48, 50, 0x000F172A);
    draw_border(fb, w, h, dash_x + 24, 334, dash_w - 48, 50, 0x00334155);

    serial_print("[GraphicsStudio] High-End Workstation Viewport rendered successfully.\n");
}
