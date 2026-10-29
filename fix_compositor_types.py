# 1. Fix spatial_compositor.h
header_code = """
#ifndef SPATIAL_COMPOSITOR_H
#define SPATIAL_COMPOSITOR_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_WINDOWS 5

typedef struct {
    int id;
    int x, y, width, height;
    char title[32];
    char status_text[32];
    uint32_t color;
    bool active;
} spatial_window_t;

void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch);
void spatial_compositor_render_frame(void);
void spatial_compositor_render(uint32_t mode);
uint32_t* get_linear_framebuffer(void);

void spatial_window_set_title(int id, const char* title);
void spatial_window_set_status(int id, const char* status);
void spatial_window_set_color(int id, uint32_t color);

#endif
"""

with open("spatial_compositor.h", "w") as f:
    f.write(header_code.strip() + "\n")

# 2. Fix spatial_compositor.c with proper field initializers
compositor_code = """
#include "spatial_compositor.h"
#include <string.h>

static uint32_t *screen_fb = (uint32_t*)0xE0000000;
static uint32_t screen_width = 1024;
static uint32_t screen_height = 768;
static uint32_t screen_pitch = 1024 * 4;
static uint32_t render_counter = 0;

static spatial_window_t windows[MAX_WINDOWS] = {
    {1, 50,  50,  300, 200, "SOT-MRAM Arrays", "ACTIVE", 0x003366, true},
    {2, 380, 50,  300, 200, "Twin-Prime Memory", "OPTIMAL", 0x663300, true},
    {3, 710, 50,  300, 200, "AuraLink Backplane", "100Gb/s", 0x006633, true},
    {4, 150, 300, 350, 220, "Triconjectural Engine", "SYNCED", 0x660066, true},
    {5, 530, 300, 350, 220, "Spatial Compositor", "ACTIVE", 0x333333, true}
};

void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch) {
    if (fb) screen_fb = fb;
    if (width > 0) screen_width = width;
    if (height > 0) screen_height = height;
    if (pitch > 0) screen_pitch = pitch;
}

uint32_t* get_linear_framebuffer(void) {
    return screen_fb;
}

void spatial_window_set_title(int id, const char* title) {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i].id == id) {
            int j = 0;
            while (title[j] != '\\0' && j < 31) {
                windows[i].title[j] = title[j];
                j++;
            }
            windows[i].title[j] = '\\0';
        }
    }
}

void spatial_window_set_status(int id, const char* status) {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i].id == id) {
            int j = 0;
            while (status[j] != '\\0' && j < 31) {
                windows[i].status_text[j] = status[j];
                j++;
            }
            windows[i].status_text[j] = '\\0';
        }
    }
}

void spatial_window_set_color(int id, uint32_t color) {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i].id == id) {
            windows[i].color = color;
        }
    }
}

static void draw_rect(int x, int y, int w, int h, uint32_t color) {
    if (!screen_fb) return;
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < (int)screen_width && py >= 0 && py < (int)screen_height) {
                screen_fb[py * (screen_pitch / 4) + px] = color;
            }
        }
    }
}

void spatial_compositor_render_frame(void) {
    if (!screen_fb) return;
    render_counter++;

    // Clear background (Dark deep space blue/gray)
    draw_rect(0, 0, screen_width, screen_height, 0x111118);

    // Render windows with dynamic pulse / focus rings
    for (int i = 0; i < MAX_WINDOWS; i++) {
        spatial_window_t *w = &windows[i];
        if (!w->active) continue;

        uint32_t border_color = ((render_counter / 15) % MAX_WINDOWS == (uint32_t)i) ? 0x00FF88 : 0xCCCCCC;

        // Window border
        draw_rect(w->x, w->y, w->width, w->height, border_color);
        // Title bar
        draw_rect(w->x + 2, w->y + 2, w->width - 4, 24, 0x222244);
        // Window body
        draw_rect(w->x + 2, w->y + 26, w->width - 4, w->height - 28, w->color);
    }
}

void spatial_compositor_render(uint32_t mode) {
    (void)mode;
    spatial_compositor_render_frame();
}
"""

with open("spatial_compositor.c", "w") as f:
    f.write(compositor_code.strip() + "\n")

print("Spatial compositor header and implementation reconciled successfully!")
