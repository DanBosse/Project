#include "spatial_compositor.h"
#include "torus.h"
#include "limine/limine.h"
#include <stddef.h>
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define MAX_COMPOSITOR_INPUT 128

static char pane_buffers[4][MAX_COMPOSITOR_INPUT];
static int pane_lengths[4] = {0, 0, 0, 0};
static int active_pane = 0;

#define FALLBACK_WIDTH  1024
#define FALLBACK_HEIGHT 768
static uint32_t fallback_screen[FALLBACK_WIDTH * FALLBACK_HEIGHT];

// Compact 5x7 Font Bitmaps for ASCII characters
static const uint8_t font_5x7[128][5] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00},
    ['-'] = {0x08, 0x08, 0x08, 0x08, 0x08},
    ['_'] = {0x40, 0x40, 0x40, 0x40, 0x40},
    ['('] = {0x1C, 0x22, 0x41, 0x41, 0x00},
    [')'] = {0x41, 0x41, 0x22, 0x1C, 0x00},
    ['['] = {0x7F, 0x41, 0x41, 0x00, 0x00},
    [']'] = {0x41, 0x41, 0x7F, 0x00, 0x00},
    [':'] = {0x00, 0x36, 0x36, 0x00, 0x00},
    ['.'] = {0x60, 0x60, 0x00, 0x00, 0x00},
    ['>'] = {0x41, 0x22, 0x14, 0x08, 0x00},
    ['0'] = {0x3E, 0x51, 0x49, 0x45, 0x3E},
    ['1'] = {0x00, 0x42, 0x7F, 0x40, 0x00},
    ['2'] = {0x42, 0x61, 0x51, 0x49, 0x46},
    ['3'] = {0x21, 0x41, 0x45, 0x4B, 0x31},
    ['4'] = {0x18, 0x14, 0x12, 0x7F, 0x10},
    ['5'] = {0x27, 0x45, 0x45, 0x45, 0x39},
    ['6'] = {0x3C, 0x4A, 0x49, 0x49, 0x30},
    ['7'] = {0x01, 0x71, 0x09, 0x05, 0x03},
    ['8'] = {0x36, 0x49, 0x49, 0x49, 0x36},
    ['9'] = {0x06, 0x49, 0x49, 0x29, 0x1E},
    ['A'] = {0x7C, 0x12, 0x11, 0x12, 0x7C},
    ['B'] = {0x7F, 0x49, 0x49, 0x49, 0x36},
    ['C'] = {0x3E, 0x41, 0x41, 0x41, 0x22},
    ['D'] = {0x7F, 0x41, 0x41, 0x22, 0x1C},
    ['E'] = {0x7F, 0x49, 0x49, 0x49, 0x41},
    ['F'] = {0x7F, 0x09, 0x09, 0x09, 0x01},
    ['G'] = {0x3E, 0x41, 0x49, 0x49, 0x7A},
    ['H'] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['I'] = {0x41, 0x7F, 0x41, 0x00, 0x00},
    ['J'] = {0x20, 0x40, 0x41, 0x3F, 0x01},
    ['K'] = {0x7F, 0x08, 0x14, 0x22, 0x41},
    ['L'] = {0x7F, 0x40, 0x40, 0x40, 0x40},
    ['M'] = {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    ['N'] = {0x7F, 0x04, 0x08, 0x10, 0x7F},
    ['O'] = {0x3E, 0x41, 0x41, 0x41, 0x3E},
    ['P'] = {0x7F, 0x09, 0x09, 0x09, 0x06},
    ['Q'] = {0x3E, 0x41, 0x51, 0x21, 0x5E},
    ['R'] = {0x7F, 0x09, 0x19, 0x29, 0x46},
    ['S'] = {0x26, 0x49, 0x49, 0x49, 0x32},
    ['T'] = {0x01, 0x01, 0x7F, 0x01, 0x01},
    ['U'] = {0x3F, 0x40, 0x40, 0x40, 0x3F},
    ['V'] = {0x1F, 0x20, 0x40, 0x20, 0x1F},
    ['W'] = {0x3F, 0x40, 0x30, 0x40, 0x3F},
    ['X'] = {0x41, 0x22, 0x1C, 0x22, 0x41},
    ['Y'] = {0x01, 0x02, 0x7C, 0x02, 0x01},
    ['Z'] = {0x61, 0x51, 0x49, 0x45, 0x43},
    // lowercase fallback
    ['a'] = {0x7C, 0x12, 0x11, 0x12, 0x7C},
    ['b'] = {0x7F, 0x49, 0x49, 0x49, 0x36},
    ['c'] = {0x3E, 0x41, 0x41, 0x41, 0x22},
    ['d'] = {0x7F, 0x41, 0x41, 0x22, 0x1C},
    ['e'] = {0x7F, 0x49, 0x49, 0x49, 0x41},
    ['f'] = {0x7F, 0x09, 0x09, 0x09, 0x01},
    ['g'] = {0x3E, 0x41, 0x49, 0x49, 0x7A},
    ['h'] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['i'] = {0x41, 0x7F, 0x41, 0x00, 0x00},
    ['j'] = {0x20, 0x40, 0x41, 0x3F, 0x01},
    ['k'] = {0x7F, 0x08, 0x14, 0x22, 0x41},
    ['l'] = {0x7F, 0x40, 0x40, 0x40, 0x40},
    ['m'] = {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    ['n'] = {0x7F, 0x04, 0x08, 0x10, 0x7F},
    ['o'] = {0x3E, 0x41, 0x41, 0x41, 0x3E},
    ['p'] = {0x7F, 0x09, 0x09, 0x09, 0x06},
    ['q'] = {0x3E, 0x41, 0x51, 0x21, 0x5E},
    ['r'] = {0x7F, 0x09, 0x19, 0x29, 0x46},
    ['s'] = {0x26, 0x49, 0x49, 0x49, 0x32},
    ['t'] = {0x01, 0x01, 0x7F, 0x01, 0x01},
    ['u'] = {0x3F, 0x40, 0x40, 0x40, 0x3F},
    ['v'] = {0x1F, 0x20, 0x40, 0x20, 0x1F},
    ['w'] = {0x3F, 0x40, 0x30, 0x40, 0x3F},
    ['x'] = {0x41, 0x22, 0x1C, 0x22, 0x41},
    ['y'] = {0x01, 0x02, 0x7C, 0x02, 0x01},
    ['z'] = {0x61, 0x51, 0x49, 0x45, 0x43}
};

void spatial_compositor_init(void) {
    for (int i = 0; i < 4; i++) {
        pane_buffers[i][0] = '\0';
        pane_lengths[i] = 0;
    }
    active_pane = 0;
    serial_print("[SpatialCompositor] Initializing Full Multi-Line & Cursor Compositor...\n");
}

void spatial_compositor_feed_input(char c) {
    if (c == '\t') {
        active_pane = (active_pane + 1) % 4;
        serial_print("[SpatialCompositor] Focus shifted to pane: ");
        char buf[4] = {(char)('0' + active_pane), '\n', '\0'};
        serial_print(buf);
        spatial_compositor_render(0);
        return;
    }

    int len = pane_lengths[active_pane];
    char *buf = pane_buffers[active_pane];

    if (c == '\n' || c == '\r') {
        if (len < MAX_COMPOSITOR_INPUT - 2) {
            buf[len++] = '\n';
            buf[len] = '\0';
            pane_lengths[active_pane] = len;
        }
    } else if (c == '\b' || c == 0x7f) {
        if (len > 0) {
            len--;
            pane_lengths[active_pane] = len;
            buf[len] = '\0';
        }
    } else {
        if (len < MAX_COMPOSITOR_INPUT - 1) {
            buf[len++] = c;
            pane_lengths[active_pane] = len;
            buf[len] = '\0';
        }
    }
    spatial_compositor_render(0);
}

void spatial_compositor_update_scancode(uint8_t scancode) {
    (void)scancode;
}

void spatial_compositor_handle_char(char c) {
    spatial_compositor_feed_input(c);
}

static void draw_rect(uint32_t *fb, uint32_t screen_width, uint32_t screen_height, 
                      uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < screen_height; y++) {
        for (uint32_t x = rx; x < rx + rw && x < screen_width; x++) {
            fb[y * screen_width + x] = color;
        }
    }
}

static void draw_char(uint32_t *fb, uint32_t width, uint32_t height, uint32_t cx, uint32_t cy, char c, uint32_t color) {
    unsigned char uc = (unsigned char)c;
    const uint8_t *glyph = font_5x7[uc];
    
    for (int px = 0; px < 5; px++) {
        uint8_t col_bits = glyph[px];
        for (int py = 0; py < 7; py++) {
            if (col_bits & (1 << py)) {
                uint32_t px_x = cx + (px * 2);
                uint32_t px_y = cy + (py * 2);
                if (px_x + 1 < width && px_y + 1 < height) {
                    fb[px_y * width + px_x] = color;
                    fb[px_y * width + (px_x + 1)] = color;
                    fb[(px_y + 1) * width + px_x] = color;
                    fb[(px_y + 1) * width + (px_x + 1)] = color;
                }
            }
        }
    }
}

static void draw_string(uint32_t *fb, uint32_t width, uint32_t height, uint32_t sx, uint32_t sy, const char *str, uint32_t color) {
    uint32_t cx = sx;
    uint32_t cy = sy;
    while (*str) {
        if (*str == '\n') {
            cy += 18;
            cx = sx;
        } else {
            draw_char(fb, width, height, cx, cy, *str, color);
            cx += 14;
            if (cx > sx + 380) { // Wrap line automatically
                cx = sx;
                cy += 18;
            }
        }
        str++;
    }
}

// Render multi-line buffer with active cursor
static void draw_pane_content(uint32_t *fb, uint32_t width, uint32_t height, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, int pane_idx, int is_focused) {
    uint32_t sx = rx + 16;
    uint32_t sy = ry + 44;
    uint32_t max_x = rx + rw - 16;
    uint32_t max_y = ry + rh - 20;

    uint32_t cx = sx;
    uint32_t cy = sy;

    const char *str = pane_buffers[pane_idx];
    if (pane_lengths[pane_idx] == 0) {
        if (is_focused) {
            draw_string(fb, width, height, sx, sy, "> _", 0x0038BDF8);
        } else {
            draw_string(fb, width, height, sx, sy, "(idle buffer)", 0x0064748B);
        }
        return;
    }

    while (*str && cy < max_y) {
        if (*str == '\n') {
            cy += 18;
            cx = sx;
        } else {
            draw_char(fb, width, height, cx, cy, *str, 0x00F8FAFC);
            cx += 14;
            if (cx > max_x) {
                cx = sx;
                cy += 18;
            }
        }
        str++;
    }

    // Draw active blinking cursor at the end of text if focused
    if (is_focused && cy < max_y) {
        draw_rect(fb, width, height, cx, cy + 2, 10, 14, 0x0038BDF8);
    }
}

static void draw_window_pane(uint32_t *fb, uint32_t width, uint32_t height,
                             uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh,
                             int pane_idx, int is_focused) {
    uint32_t bg_color = is_focused ? 0x001E293B : 0x000F172A;
    draw_rect(fb, width, height, rx, ry, rw, rh, bg_color);
    
    uint32_t title_color = is_focused ? 0x00334155 : 0x001E293B;
    draw_rect(fb, width, height, rx, ry, rw, 28, title_color);

    uint32_t border_color = is_focused ? 0x0038BDF8 : 0x00334155;
    for (uint32_t x = rx; x < rx + rw; x++) {
        fb[ry * width + x] = border_color;
        fb[(ry + rh - 1) * width + x] = border_color;
    }
    for (uint32_t y = ry; y < ry + rh; y++) {
        fb[y * width + rx] = border_color;
        fb[y * width + (rx + rw - 1)] = border_color;
    }

    if (pane_idx == 0) draw_string(fb, width, height, rx + 12, ry + 8, "PANE 0 [ACTIVE]", is_focused ? 0x0038BDF8 : 0x0094A3B8);
    else if (pane_idx == 1) draw_string(fb, width, height, rx + 12, ry + 8, "PANE 1", is_focused ? 0x0038BDF8 : 0x0094A3B8);
    else if (pane_idx == 2) draw_string(fb, width, height, rx + 12, ry + 8, "PANE 2", is_focused ? 0x0038BDF8 : 0x0094A3B8);
    else if (pane_idx == 3) draw_string(fb, width, height, rx + 12, ry + 8, "PANE 3", is_focused ? 0x0038BDF8 : 0x0094A3B8);

    draw_pane_content(fb, width, height, rx, ry, rw, rh, pane_idx, is_focused);
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

    draw_rect(fb, width, height, 0, 0, width, height, 0x000B0F19);

    uint32_t half_w = width / 2;
    uint32_t half_h = (height - 40) / 2;

    draw_window_pane(fb, width, height, 20, 20, half_w - 30, half_h - 20, 0, active_pane == 0);
    draw_window_pane(fb, width, height, half_w + 10, 20, half_w - 30, half_h - 20, 1, active_pane == 1);
    draw_window_pane(fb, width, height, 20, half_h + 10, half_w - 30, half_h - 20, 2, active_pane == 2);
    draw_window_pane(fb, width, height, half_w + 10, half_h + 10, half_w - 30, half_h - 20, 3, active_pane == 3);

    // Status bar
    draw_rect(fb, width, height, 0, height - 36, width, 36, 0x00020617);
    draw_string(fb, width, height, 24, height - 26, "TORUSOS TIER 6 STUDIO LENS | MULTI-LINE SPATIAL WORKSPACE", 0x0038BDF8);
}
