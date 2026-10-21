#include "spatial_compositor.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

void spatial_compositor_init(void) {
    serial_print("[SpatialCompositor] Initializing Tier 6 Spatial Window & Compositor Engine...\n");
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            if (x < w && y < h) {
                fb[y * w + x] = color;
            }
        }
    }
}

// Standalone kernel-safe math approximations for embedded raymarching
static float kernel_sqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    float res = x;
    for (int i = 0; i < 6; i++) {
        res = 0.5f * (res + x / res);
    }
    return res;
}

static float kernel_pow4(float base) {
    float b2 = base * base;
    return b2 * b2;
}

static void render_embedded_shader(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
    float cx = start_x + width / 2.0f;
    float cy = start_y + height / 2.0f;
    float radius = (width < height ? width : height) * 0.42f;

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t px = start_x + x;
            uint32_t py = start_y + y;
            if (px >= w || py >= h) continue;

            float nx = ((float)px - cx) / radius;
            float ny = ((float)py - cy) / radius;
            float dist_sq = nx * nx + ny * ny;

            if (dist_sq <= 1.0f) {
                float nz = kernel_sqrt(1.0f - dist_sq);

                float lx = -0.577f;
                float ly = -0.577f;
                float lz = 0.577f;

                float diff = nx * (-lx) + ny * (-ly) + nz * lz;
                if (diff < 0.05f) diff = 0.05f;

                float spec = kernel_pow4(kernel_pow4(diff));

                uint8_t red = (uint8_t)((30 + 150 * diff + 75 * spec));
                uint8_t green = (uint8_t)((10 + 100 * diff + 150 * spec));
                uint8_t blue = (uint8_t)((80 + 175 * diff + 50 * spec));

                uint32_t color = (0xFF << 24) | (red << 16) | (green << 8) | blue;
                fb[py * w + px] = color;
            } else {
                uint8_t bg = (uint8_t)(16 + (y * 8 / height));
                uint32_t bg_color = (0xFF << 24) | (bg << 16) | ((bg + 4) << 8) | (bg + 12);
                fb[py * w + px] = bg_color;
            }
        }
    }
}

void spatial_compositor_render(uint32_t flags) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;

    // 1. Clear Desktop Workspace Background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000B0F19);

    if (flags == 0) {
        // --- FLOATING WINDOW WITH EMBEDDED GRAPHICS STUDIO ---
        uint32_t win_x = 100, win_y = 80, win_w = 824, win_h = 600;

        // Window Drop Shadow
        draw_rect(fb, WIDTH, HEIGHT, win_x + 8, win_y + 8, win_w, win_h, 0x0003070E);

        // Window Outer Border / Frame
        draw_rect(fb, WIDTH, HEIGHT, win_x, win_y, win_w, win_h, 0x00334155);

        // Window Interior Body Background (Studio Workspace)
        draw_rect(fb, WIDTH, HEIGHT, win_x + 2, win_y + 32, win_w - 4, win_h - 34, 0x000F172A);

        // Window Title Bar
        draw_rect(fb, WIDTH, HEIGHT, win_x + 2, win_y + 2, win_w - 4, 30, 0x001E293B);

        // Window Control Buttons (Header dots on top right)
        // Close Button (Red)
        draw_rect(fb, WIDTH, HEIGHT, win_x + win_w - 24, win_y + 10, 12, 12, 0x00EF4444);
        // Minimize Button (Yellow)
        draw_rect(fb, WIDTH, HEIGHT, win_x + win_w - 44, win_y + 10, 12, 12, 0x00F59E0B);
        // Expand/Maximize Button (Green)
        draw_rect(fb, WIDTH, HEIGHT, win_x + win_w - 64, win_y + 10, 12, 12, 0x0010B981);

        // --- EMBEDDED STUDIO CONTENT ---
        // Left Tool Palette inside Window
        draw_rect(fb, WIDTH, HEIGHT, win_x + 16, win_y + 48, 70, win_h - 64, 0x001E293B);

        // Central Artboard Canvas inside Window
        draw_rect(fb, WIDTH, HEIGHT, win_x + 98, win_y + 48, win_w - 114, win_h - 64, 0x00FFFFFF);

        // Render High-End Raymarched 3D Shader inside the Window's Artboard!
        render_embedded_shader(fb, WIDTH, HEIGHT, win_x + 114, win_y + 64, win_w - 146, win_h - 96);

        serial_print("[SpatialCompositor] Rendered floating window with embedded Graphics Studio content.\n");
    } else {
        // --- GRID LAYOUT MODE (2x2 Multi-Studio Split) ---
        uint32_t half_w = WIDTH / 2;
        uint32_t half_h = HEIGHT / 2;

        draw_rect(fb, WIDTH, HEIGHT, 10, 10, half_w - 15, half_h - 15, 0x001E293B);
        draw_rect(fb, WIDTH, HEIGHT, half_w + 5, 10, half_w - 15, half_h - 15, 0x001E293B);
        draw_rect(fb, WIDTH, HEIGHT, 10, half_h + 5, half_w - 15, half_h - 15, 0x001E293B);
        draw_rect(fb, WIDTH, HEIGHT, half_w + 5, half_h + 5, half_w - 15, half_h - 15, 0x001E293B);

        serial_print("[SpatialCompositor] Rendered 2x2 grid multi-studio split layout.\n");
    }
}

// Input handling stubs for keyboard integration (HID)
void spatial_compositor_update_scancode(uint8_t scancode) {
    (void)scancode;
}

void spatial_compositor_handle_char(char c) {
    (void)c;
}
