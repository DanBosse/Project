#include "spatial_compositor.h"
#include "torus.h"
#include "limine/limine.h"
#include <stdint.h>
#include <string.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768
#define MAX_WINDOWS 8

static SpatialWindow window_stack[MAX_WINDOWS];
static int window_count = 0;
static int active_window_index = 0;

static int32_t mouse_x = WIDTH / 2;
static int32_t mouse_y = HEIGHT / 2;
static uint8_t prev_mouse_buttons = 0;
static uint32_t frame_tick_counter = 0;

void spatial_compositor_init(void) {
    serial_print("[SpatialCompositor] Initializing Tier 6 Spatial Window & Compositor Engine...\n");
    window_count = 0;
    spatial_compositor_create_window("Graphics Studio", 60, 50, 440, 320);
    spatial_compositor_create_window("Telemetry Panel", 520, 50, 440, 320);
    spatial_compositor_create_window("Audio Workstation", 60, 390, 440, 320);
    spatial_compositor_create_window("Video NLE Timeline", 520, 390, 440, 320);
}

int spatial_compositor_create_window(const char* title, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    if (window_count >= MAX_WINDOWS) return -1;
    int idx = window_count++;
    int i = 0;
    while (title && title[i] != '\0' && i < 31) {
        window_stack[idx].title[i] = title[i];
        i++;
    }
    window_stack[idx].title[i] = '\0';
    window_stack[idx].x = x;
    window_stack[idx].y = y;
    window_stack[idx].w = w;
    window_stack[idx].h = h;
    window_stack[idx].z_index = idx;
    window_stack[idx].is_active = (idx == active_window_index);
    window_stack[idx].is_minimized = 0;
    return idx;
}

void spatial_compositor_focus_window(int index) {
    if (index < 0 || index >= window_count) return;
    active_window_index = index;
    for (int i = 0; i < window_count; i++) {
        window_stack[i].is_active = (i == active_window_index);
    }
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

static const uint8_t font_5x7[128][7] = {
    ['A'] = {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    ['B'] = {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},
    ['C'] = {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E},
    ['D'] = {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E},
    ['E'] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},
    ['F'] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10},
    ['G'] = {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0E},
    ['H'] = {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    ['I'] = {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},
    ['J'] = {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C},
    ['K'] = {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},
    ['L'] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F},
    ['M'] = {0x11, 0x1B, 0x15, 0x11, 0x11, 0x11, 0x11},
    ['N'] = {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11},
    ['O'] = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    ['P'] = {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10},
    ['Q'] = {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D},
    ['R'] = {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11},
    ['S'] = {0x0E, 0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E},
    ['T'] = {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},
    ['U'] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    ['V'] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04},
    ['W'] = {0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11},
    ['X'] = {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11},
    ['Y'] = {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04},
    ['Z'] = {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F},
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    [':'] = {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00},
    ['-'] = {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00},
    ['_'] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F},
};

static void draw_char(uint32_t *fb, uint32_t w, uint32_t h, int x, int y, char c, uint32_t color) {
    if (c >= 'a' && c <= 'z') c -= 32;
    unsigned char uc = (unsigned char)c;
    if (uc >= 128) uc = ' ';
    for (int row = 0; row < 7; row++) {
        uint8_t bits = font_5x7[uc][row];
        for (int col = 0; col < 5; col++) {
            if (bits & (1 << (4 - col))) {
                int px = x + col;
                int py = y + row;
                if (px >= 0 && px < (int32_t)w && py >= 0 && py < (int32_t)h) {
                    fb[py * w + px] = color;
                }
            }
        }
    }
}

static void draw_string(uint32_t *fb, uint32_t w, uint32_t h, int x, int y, const char *str, uint32_t color) {
    int cur_x = x;
    while (*str) {
        draw_char(fb, w, h, cur_x, y, *str, color);
        cur_x += 6;
        str++;
    }
}

static void render_cursor(uint32_t *fb, uint32_t w, uint32_t h, int32_t cx, int32_t cy) {
    for (int dy = 0; dy < 12; dy++) {
        for (int dx = 0; dx < 8; dx++) {
            if (dx <= dy) {
                int32_t px = cx + dx;
                int32_t py = cy + dy;
                if (px >= 0 && px < (int32_t)w && py >= 0 && py < (int32_t)h) {
                    fb[py * w + px] = (dx == 0 || dy == 11 || dx == dy) ? 0x00000000 : 0x00FFFFFF;
                }
            }
        }
    }
}

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
                float lx = -0.577f, ly = -0.577f, lz = 0.577f;
                float diff = nx * (-lx) + ny * (-ly) + nz * lz;
                if (diff < 0.05f) diff = 0.05f;
                float spec = kernel_pow4(kernel_pow4(diff));

                uint8_t red = (uint8_t)((30 + 150 * diff + 75 * spec));
                uint8_t green = (uint8_t)((10 + 100 * diff + 150 * spec));
                uint8_t blue = (uint8_t)((80 + 175 * diff + 50 * spec));

                fb[py * w + px] = (0xFF << 24) | (red << 16) | (green << 8) | blue;
            } else {
                uint8_t bg = (uint8_t)(16 + (y * 8 / height));
                fb[py * w + px] = (0xFF << 24) | (bg << 16) | ((bg + 4) << 8) | (bg + 12);
            }
        }
    }
}

static void render_telemetry_dashboard(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
    draw_rect(fb, w, h, start_x, start_y, width, height, 0x000B0F19);
    draw_string(fb, w, h, start_x + 16, start_y + 12, "CPU LOAD", 0x00FFFFFF);

    uint32_t cpu_load_w = 80 + (frame_tick_counter % 60);
    draw_rect(fb, w, h, start_x + 16, start_y + 32, width - 32, 14, 0x001E293B);
    draw_rect(fb, w, h, start_x + 16, start_y + 32, cpu_load_w > width - 32 ? width - 32 : cpu_load_w, 14, 0x0010B981);

    draw_string(fb, w, h, start_x + 16, start_y + 60, "RAM ALLOCATED", 0x00FFFFFF);
    draw_rect(fb, w, h, start_x + 16, start_y + 80, width - 32, 14, 0x001E293B);
    draw_rect(fb, w, h, start_x + 16, start_y + 80, (width - 32) * 3 / 5, 14, 0x003B82F6);

    draw_string(fb, w, h, start_x + 16, start_y + 108, "ACTIVE THREADS", 0x00FFFFFF);
    for (int i = 0; i < 4; i++) {
        uint32_t bar_color = (i < window_count) ? 0x008B5CF6 : 0x00334155;
        draw_rect(fb, w, h, start_x + 16 + (i * 50), start_y + 128, 42, 22, bar_color);
    }
}

static void render_audio_workstation(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
    draw_rect(fb, w, h, start_x, start_y, width, height, 0x000B0F19);
    draw_string(fb, w, h, start_x + 16, start_y + 12, "SPECTRUM ANALYZER", 0x0038BDF8);

    int num_bars = 16;
    uint32_t bar_w = (width - 32) / num_bars;
    for (int i = 0; i < num_bars; i++) {
        uint32_t bar_h = 15 + ((i * 13 + frame_tick_counter * (i % 3 + 1)) % (height - 60));
        uint32_t bx = start_x + 16 + (i * bar_w);
        uint32_t by = start_y + height - 24 - bar_h;
        draw_rect(fb, w, h, bx, by, bar_w - 3, bar_h, 0x0038BDF8);
    }
}

static void render_video_nle(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
    draw_rect(fb, w, h, start_x, start_y, width, height, 0x000B0F19);
    draw_string(fb, w, h, start_x + 16, start_y + 12, "TIMELINE TRACKS", 0x00F43F5E);

    draw_rect(fb, w, h, start_x + 16, start_y + 40, width - 32, 28, 0x001E293B);
    draw_rect(fb, w, h, start_x + 24, start_y + 44, 90, 20, 0x00E11D48);
    draw_rect(fb, w, h, start_x + 120, start_y + 44, 120, 20, 0x00FB7185);

    draw_rect(fb, w, h, start_x + 16, start_y + 76, width - 32, 28, 0x001E293B);
    draw_rect(fb, w, h, start_x + 24, start_y + 80, 216, 20, 0x000284C7);

    uint32_t playhead_x = start_x + 24 + ((frame_tick_counter * 2) % (width - 60));
    draw_rect(fb, w, h, playhead_x, start_y + 32, 2, height - 40, 0x00FACC15);
}

void spatial_compositor_render(uint32_t flags) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;
    frame_tick_counter++;

    draw_rect(fb, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, 0x000B0F19);

    if (flags == LAYOUT_FLOATING) {
        for (int i = 0; i < window_count; i++) {
            SpatialWindow *win = &window_stack[i];
            if (win->is_minimized) continue;

            uint32_t wx = win->x, wy = win->y, ww = win->w, wh = win->h;

            draw_rect(fb, WIDTH, HEIGHT, wx + 6, wy + 6, ww, wh, win->is_active ? 0x0003070E : 0x00010306);
            draw_rect(fb, WIDTH, HEIGHT, wx, wy, ww, wh, win->is_active ? 0x00475569 : 0x001E293B);
            draw_rect(fb, WIDTH, HEIGHT, wx + 2, wy + 26, ww - 4, wh - 28, 0x000F172A);
            draw_rect(fb, WIDTH, HEIGHT, wx + 2, wy + 2, ww - 4, 24, win->is_active ? 0x001E293B : 0x000F172A);

            draw_string(fb, WIDTH, HEIGHT, wx + 12, wy + 8, win->title, 0x00FFFFFF);

            draw_rect(fb, WIDTH, HEIGHT, wx + ww - 20, wy + 8, 10, 10, 0x00EF4444);
            draw_rect(fb, WIDTH, HEIGHT, wx + ww - 36, wy + 8, 10, 10, 0x00F59E0B);
            draw_rect(fb, WIDTH, HEIGHT, wx + ww - 52, wy + 8, 10, 10, 0x0010B981);

            uint32_t content_x = wx + 12;
            uint32_t content_y = wy + 34;
            uint32_t content_w = ww - 24;
            uint32_t content_h = wh - 42;

            if (i == 0) {
                render_embedded_shader(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 1) {
                render_telemetry_dashboard(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 2) {
                render_audio_workstation(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 3) {
                render_video_nle(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            }
        }
    } else {
        uint32_t half_w = WIDTH / 2;
        uint32_t half_h = HEIGHT / 2;
        uint32_t pad = 8;

        // Q1: Graphics Studio
        draw_rect(fb, WIDTH, HEIGHT, pad, pad, half_w - (pad * 3 / 2), half_h - (pad * 3 / 2), 0x000F172A);
        draw_rect(fb, WIDTH, HEIGHT, pad, pad, half_w - (pad * 3 / 2), 28, 0x001E293B);
        draw_string(fb, WIDTH, HEIGHT, pad + 12, pad + 8, "GRAPHICS STUDIO", 0x00FFFFFF);
        render_embedded_shader(fb, WIDTH, HEIGHT, pad + 12, pad + 34, half_w - (pad * 3 / 2) - 24, half_h - (pad * 3 / 2) - 42);

        // Q2: Telemetry Panel
        draw_rect(fb, WIDTH, HEIGHT, half_w + (pad / 2), pad, half_w - (pad * 3 / 2), half_h - (pad * 3 / 2), 0x000F172A);
        draw_rect(fb, WIDTH, HEIGHT, half_w + (pad / 2), pad, half_w - (pad * 3 / 2), 28, 0x001E293B);
        draw_string(fb, WIDTH, HEIGHT, half_w + (pad / 2) + 12, pad + 8, "TELEMETRY PANEL", 0x00FFFFFF);
        render_telemetry_dashboard(fb, WIDTH, HEIGHT, half_w + (pad / 2) + 12, pad + 34, half_w - (pad * 3 / 2) - 24, half_h - (pad * 3 / 2) - 42);

        // Q3: Audio Workstation
        draw_rect(fb, WIDTH, HEIGHT, pad, half_h + (pad / 2), half_w - (pad * 3 / 2), half_h - (pad * 3 / 2), 0x000F172A);
        draw_rect(fb, WIDTH, HEIGHT, pad, half_h + (pad / 2), half_w - (pad * 3 / 2), 28, 0x001E293B);
        draw_string(fb, WIDTH, HEIGHT, pad + 12, half_h + (pad / 2) + 8, "AUDIO WORKSTATION", 0x00FFFFFF);
        render_audio_workstation(fb, WIDTH, HEIGHT, pad + 12, half_h + (pad / 2) + 34, half_w - (pad * 3 / 2) - 24, half_h - (pad * 3 / 2) - 42);

        // Q4: Video NLE
        draw_rect(fb, WIDTH, HEIGHT, half_w + (pad / 2), half_h + (pad / 2), half_w - (pad * 3 / 2), half_h - (pad * 3 / 2), 0x000F172A);
        draw_rect(fb, WIDTH, HEIGHT, half_w + (pad / 2), half_h + (pad / 2), half_w - (pad * 3 / 2), 28, 0x001E293B);
        draw_string(fb, WIDTH, HEIGHT, half_w + (pad / 2) + 12, half_h + (pad / 2) + 8, "VIDEO NLE TIMELINE", 0x00FFFFFF);
        render_video_nle(fb, WIDTH, HEIGHT, half_w + (pad / 2) + 12, half_h + (pad / 2) + 34, half_w - (pad * 3 / 2) - 24, half_h - (pad * 3 / 2) - 42);
    }

    render_cursor(fb, WIDTH, HEIGHT, mouse_x, mouse_y);
}

void spatial_compositor_mouse_move(int32_t x, int32_t y) {
    mouse_x = x;
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= WIDTH) mouse_x = WIDTH - 1;
    mouse_y = y;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= HEIGHT) mouse_y = HEIGHT - 1;
}

void spatial_compositor_mouse_click(int32_t x, int32_t y, uint8_t button_state) {
    if (button_state && !prev_mouse_buttons) {
        for (int i = window_count - 1; i >= 0; i--) {
            SpatialWindow *win = &window_stack[i];
            if (win->is_minimized) continue;
            if (x >= (int32_t)win->x && x < (int32_t)(win->x + win->w) &&
                y >= (int32_t)win->y && y < (int32_t)(win->y + win->h)) {
                spatial_compositor_focus_window(i);
                break;
            }
        }
    }
    prev_mouse_buttons = button_state;
}

void spatial_compositor_update_scancode(uint8_t scancode) { (void)scancode; }
void spatial_compositor_handle_char(char c) { (void)c; }
