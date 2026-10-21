#include "graphics.h"

// Limine Framebuffer Request structures
struct limine_framebuffer {
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
};

struct limine_framebuffer_response {
    uint64_t revision;
    uint64_t framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

static Framebuffer fb;
static int graphics_initialized = 0;

void graphics_init(void* response) {
    if (!response) return;
    
    struct limine_framebuffer_response* fb_resp = (struct limine_framebuffer_response*)response;
    if (fb_resp->framebuffer_count == 0) return;

    struct limine_framebuffer* raw_fb = fb_resp->framebuffers[0];
    
    fb.base_address = (uint32_t*)raw_fb->address;
    fb.width = raw_fb->width;
    fb.height = raw_fb->height;
    fb.pitch = raw_fb->pitch;
    fb.bpp = raw_fb->bpp;
    
    graphics_initialized = 1;
}

void put_pixel(int x, int y, uint32_t color) {
    if (!graphics_initialized) return;
    if (x < 0 || x >= (int)fb.width || y < 0 || y >= (int)fb.height) return;
    
    uint32_t* pixel_ptr = (uint32_t*)((uint8_t*)fb.base_address + y * fb.pitch + x * (fb.bpp / 8));
    *pixel_ptr = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    if (!graphics_initialized) return;
    for (int cy = y; cy < y + h; cy++) {
        for (int cx = x; cx < x + w; cx++) {
            put_pixel(cx, cy, color);
        }
    }
}

void clear_screen(uint32_t color) {
    if (!graphics_initialized) return;
    draw_rect(0, 0, fb.width, fb.height, color);
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

// High-end procedural shader: Real-time Raymarched 3D Sphere with Specular Lighting
void render_high_end_shader(int start_x, int start_y, int width, int height) {
    if (!graphics_initialized) return;

    float cx = start_x + width / 2.0f;
    float cy = start_y + height / 2.0f;
    float radius = (width < height ? width : height) * 0.35f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float px = start_x + x;
            float py = start_y + y;

            float nx = (px - cx) / radius;
            float ny = (py - cy) / radius;
            float dist_sq = nx * nx + ny * ny;

            if (dist_sq <= 1.0f) {
                float nz = kernel_sqrt(1.0f - dist_sq);

                float lx = -0.577f;
                float ly = -0.577f;
                float lz = 0.577f;

                float diff = nx * (-lx) + ny * (-ly) + nz * lz;
                if (diff < 0.05f) diff = 0.05f;

                float spec = kernel_pow4(kernel_pow4(diff)); // equivalent to diff^16

                uint8_t red = (uint8_t)((30 + 150 * diff + 75 * spec));
                uint8_t green = (uint8_t)((10 + 100 * diff + 150 * spec));
                uint8_t blue = (uint8_t)((80 + 175 * diff + 50 * spec));

                uint32_t color = (0xFF << 24) | (red << 16) | (green << 8) | blue;
                put_pixel(px, py, color);
            } else {
                uint8_t bg = (uint8_t)(16 + (y * 8 / height));
                uint32_t bg_color = (0xFF << 24) | (bg << 16) | ((bg + 4) << 8) | (bg + 12);
                put_pixel(px, py, bg_color);
            }
        }
    }
}
