#include "office_suite.h"
#include "torus.h"
#include "limine/limine.h"
#include <stddef.h>
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

static char document_buffer[1024] = "TORUSOS SOVEREIGN OFFICE SUITE\n[Mode: StudioLens Document Workspace]\n> Document loaded: Master_Architecture_v6.doc\n> Status: Ready for secure editing.\n";
static int doc_len = 135;

void office_suite_init(void) {
    serial_print("[OfficeSuite] Initializing StudioLens Sovereign Office Suite...\n");
}

void office_suite_type_char(char c) {
    if (doc_len < 1023) {
        document_buffer[doc_len++] = c;
        document_buffer[doc_len] = '\0';
    }
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

void office_suite_render(void) {
    uint32_t width = WIDTH;
    uint32_t height = HEIGHT;
    uint32_t *fb = NULL;

    if (framebuffer_request.response != NULL && framebuffer_request.response->framebuffer_count > 0) {
        struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
        fb = (uint32_t*)fb_info->address;
        width = fb_info->width;
        height = fb_info->height;
    }

    if (!fb) return;

    // Workspace Canvas
    draw_rect(fb, width, height, 0, 0, width, height, 0x000F172A);

    // Document Window Sheet (StudioLens Office Window)
    draw_rect(fb, width, height, 80, 50, width - 160, height - 100, 0x001E293B);

    // Title Bar
    draw_rect(fb, width, height, 80, 50, width - 160, 40, 0x00334155);

    // Ribbon Toolbar
    draw_rect(fb, width, height, 80, 90, width - 160, 36, 0x000B0F19);

    // Status Footer
    draw_rect(fb, width, height, 80, height - 90, width - 160, 40, 0x000B0F19);

    serial_print("[OfficeSuite] StudioLens Office workspace rendered.\n");
}
