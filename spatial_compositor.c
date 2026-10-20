#include "spatial_compositor.h"
#include "torus.h"
#include "limine/limine.h"
#include <stddef.h>
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define MAX_COMPOSITOR_INPUT 128
static char window_input_buffer[MAX_COMPOSITOR_INPUT];
static int window_input_len = 0;
static int window_focused = 1;

#define FALLBACK_WIDTH  1024
#define FALLBACK_HEIGHT 768
static uint32_t fallback_screen[FALLBACK_WIDTH * FALLBACK_HEIGHT];

// Layout modes: 0 = Master/Single, 1 = Grid (2x2 Split)
static uint32_t current_layout_mode = 0;

void spatial_compositor_init(void) {
    window_input_buffer[0] = '\0';
    window_input_len = 0;
    serial_print("[SpatialCompositor] Initializing Direct LFB Spatial Compositor with Layout Routing...\n");
    if (framebuffer_request.response == NULL) {
        serial_print("[SpatialCompositor DEBUG] framebuffer_request.response is NULL! Limine did not populate response.\n");
    } else {
        serial_print("[SpatialCompositor DEBUG] framebuffer_request.response FOUND.\n");
    }
}

void spatial_compositor_feed_input(char c) {
    if (!window_focused) return;
    if (c == '\n' || c == '\r') {
        if (window_input_len > 0) {
            window_input_buffer[window_input_len] = '\0';
            serial_print("[SpatialCompositor Input] ");
            serial_print(window_input_buffer);
            serial_print("\n");
            window_input_len = 0;
            window_input_buffer[0] = '\0';
        }
    } else if (c == '\b' || c == 0x7f) {
        if (window_input_len > 0) {
            window_input_len--;
            window_input_buffer[window_input_len] = '\0';
        }
    } else {
        if (window_input_len < MAX_COMPOSITOR_INPUT - 1) {
            window_input_buffer[window_input_len++] = c;
            window_input_buffer[window_input_len] = '\0';
        }
    }
}

void spatial_compositor_update_scancode(uint8_t scancode) {
    (void)scancode;
}

void spatial_compositor_handle_char(char c) {
    spatial_compositor_feed_input(c);
}

// Helper to draw a filled rectangle on the framebuffer
static void draw_rect(uint32_t *fb, uint32_t screen_width, uint32_t screen_height, 
                      uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < screen_height; y++) {
        for (uint32_t x = rx; x < rx + rw && x < screen_width; x++) {
            fb[y * screen_width + x] = color;
        }
    }
}

void spatial_compositor_render(uint32_t flags) {
    (void)flags;
    uint32_t width = FALLBACK_WIDTH;
    uint32_t height = FALLBACK_HEIGHT;
    uint32_t *fb = fallback_screen;

    if (framebuffer_request.response != NULL) {
        if (framebuffer_request.response->framebuffer_count > 0) {
            struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
            fb = (uint32_t*)fb_info->address;
            width = fb_info->width;
            height = fb_info->height;
        }
    }

    // 1. Clear background (Sovereign Dark Theme: 0x000F172A)
    draw_rect(fb, width, height, 0, 0, width, height, 0x000F172A);

    // Check if grid mode is requested via flags or state
    // Let's render a 2x2 split layout if width/height allow
    uint32_t half_w = width / 2;
    uint32_t half_h = height / 2;

    // Window Pane 1: Telemetry Core (Top-Left)
    draw_rect(fb, width, height, 20, 20, half_w - 30, half_h - 30, 0x001E293B);
    draw_rect(fb, width, height, 20, 20, half_w - 30, 28, 0x00334155); // Title bar

    // Window Pane 2: Studio Lens Spatial View (Top-Right)
    draw_rect(fb, width, height, half_w + 10, 20, half_w - 30, half_h - 30, 0x001E293B);
    draw_rect(fb, width, height, half_w + 10, 20, half_w - 30, 28, 0x00334155); // Title bar

    // Window Pane 3: AuraScript Flow Console (Bottom-Left)
    draw_rect(fb, width, height, 20, half_h + 10, half_w - 30, half_h - 30, 0x001E293B);
    draw_rect(fb, width, height, 20, half_h + 10, half_w - 30, 28, 0x00334155); // Title bar

    // Window Pane 4: Audio Backplane / MRAM Telemetry (Bottom-Right)
    draw_rect(fb, width, height, half_w + 10, half_h + 10, half_w - 30, half_h - 30, 0x001E293B);
    draw_rect(fb, width, height, half_w + 10, half_h + 10, half_w - 30, 28, 0x00334155); // Title bar

    // Accent borders around focused window (Pane 1)
    // Draw subtle border outline
    for (uint32_t x = 20; x < half_w - 10; x++) {
        fb[20 * width + x] = 0x0038BDF8; // Cyan accent
        fb[(half_h - 10) * width + x] = 0x0038BDF8;
    }
}
