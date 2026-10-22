#include "graphics_studio.h"
#include "studio_core.h"
#include "topological.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

static uint64_t global_render_tick = 0;

void graphics_studio_init(void) {
    serial_print("[GraphicsStudio] Initializing StudioLens Vector & Raster Design Canvas with Topological Manifold Engine...\n");
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

// Standalone kernel-safe math approximations
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

// High-end procedural raymarched shader reactive to AuraLink physics AND Topological Manifold metrics
static void render_high_end_shader(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height, float physics_mod, uint64_t psi_val) {
    float cx = start_x + width / 2.0f;
    float cy = start_y + height / 2.0f;
    
    // Dynamic radius scaled live by spring position telemetry and topological psi manifold wave!
    float base_radius = (width < height ? width : height) * 0.4f;
    float topo_mod = (float)(psi_val % 30) - 15.0f;
    float radius = base_radius + (physics_mod * 0.5f) + topo_mod;
    if (radius < 20.0f) radius = 20.0f;

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

                // Color tint modulated by topological manifold state
                uint8_t topo_color_shift = (uint8_t)(psi_val & 0x3F);
                uint8_t red = (uint8_t)((30 + topo_color_shift + 150 * diff + 75 * spec));
                uint8_t green = (uint8_t)((10 + (topo_color_shift / 2) + 100 * diff + 150 * spec));
                uint8_t blue = (uint8_t)((80 + topo_color_shift + 175 * diff + 50 * spec));

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

void graphics_studio_render(void) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;

    global_render_tick++;

    // Read live physics telemetry from AuraLink Channel 0
    float metrics[8] = {0};
    uint32_t count = 0;
    char label[32] = {0};
    bool has_physics = studio_core_read_pipe(0, metrics, &count, label);
    float physics_mod = has_physics ? metrics[0] : 0.0f;

    // Evaluate Topological Manifold Wave equation
    uint64_t flex_sum = 42 + (global_render_tick % 19);
    uint64_t psi = topological_evaluate_frame(global_render_tick, 100, flex_sum, 10, 1);

    // Canvas Studio Background
    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000F172A);

    // Left Tool Palette
    draw_rect(fb, WIDTH, HEIGHT, 50, 50, 80, HEIGHT - 100, 0x001E293B);

    // Central Artboard Canvas Background
    draw_rect(fb, WIDTH, HEIGHT, 160, 50, 780, HEIGHT - 100, 0x00FFFFFF);

    // Render High-End Raymarched 3D Shader driven by Physics + Topological Manifold Engine!
    render_high_end_shader(fb, WIDTH, HEIGHT, 180, 70, 740, HEIGHT - 140, physics_mod, psi);

    serial_print("[GraphicsStudio] High-end 3D raymarched design canvas rendered with Topological Manifold & AuraLink Stream.\n");
}
