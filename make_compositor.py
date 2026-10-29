compositor_code = """
#include "spatial_compositor.h"

static uint32_t *screen_fb = (uint32_t*)0xE0000000;
static uint32_t screen_width = 1024;
static uint32_t screen_height = 768;
static uint32_t screen_pitch = 1024 * 4;
static uint32_t render_counter = 0;

static spatial_window_t windows[MAX_WINDOWS] = {
    {1, 50,  50,  300, 200, "SOT-MRAM Arrays", 0x003366, true},
    {2, 380, 50,  300, 200, "Twin-Prime Memory", 0x663300, true},
    {3, 710, 50,  300, 200, "AuraLink Backplane", 0x006633, true},
    {4, 150, 300, 350, 220, "Triconjectural Engine", 0x660066, true},
    {5, 530, 300, 350, 220, "Spatial Compositor", 0x333333, true}
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

    // Render the 5 windows with dynamic focus / pulse effect based on render_counter
    for (int i = 0; i < MAX_WINDOWS; i++) {
        spatial_window_t *w = &windows[i];
        if (!w->active) continue;

        // Dynamic active border highlight on rotating windows
        uint32_t border_color = ((render_counter / 15) % MAX_WINDOWS == i) ? 0x00FF88 : 0xCCCCCC;

        // Window border / background
        draw_rect(w->x, w->y, w->width, w->height, border_color);
        // Title bar
        draw_rect(w->x + 2, w->y + 2, w->width - 4, 24, 0x222244);
        // Window body
        draw_rect(w->x + 2, w->y + 26, w->width - 4, w->height - 28, w->color);
    }
}

void spatial_compositor_render(int mode) {
    (void)mode;
    spatial_compositor_render_frame();
}
"""

with open("spatial_compositor.c", "w") as f:
    f.write(compositor_code.strip() + "\n")

print("spatial_compositor.c written successfully!")
