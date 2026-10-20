extern void cli_execute_command(char cmd_char);

extern void omni_envelope_init(void);
extern void enterprise_telemetry_init(void);


extern void spatial_compositor_init(void);
extern void spatial_compositor_render(void);

#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b
#define LIMINE_FRAMEBUFFER_REQUEST { LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b }

#include "simd.h"
#include "tier_scheduler.h"
#include "invariants.h"
#include "tensor.h"
#include "ps2.h"
#include "ascii_xml.h"
#include "phasecore.h"
#include "phasecore_interpreter.h"
#include <stdint.h>
#include <stddef.h>

struct limine_framebuffer_response {
    uint64_t revision;
    uint64_t framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

struct limine_framebuffer_request {
    uint64_t id[4];
    uint64_t revision;
    struct limine_framebuffer_response *response;
};

extern volatile struct limine_framebuffer_request framebuffer_request;


static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

extern void handle_keyboard_interrupt(uint8_t scancode);







void kernel_print(const char* str) {
    while (*str) {
        outb(0x3F8, *str++);
    }
}

void kernel_print_hex(uint64_t val) {
    char buf[19];
    buf[18] = '\0';
    for (int i = 15; i >= 0; i--) {
        int nibble = val & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('a' + nibble - 10);
        val >>= 4;
    }
    buf[0] = '0';
    buf[1] = 'x';
    kernel_print(buf);
}
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





__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[3] = { 0xf956242d5dfb66d3, 0x507737888efa030d, 0 };

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start[4] = {0};



__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end[4] = {0};

static int32_t cursor_x = 400;
static int32_t cursor_y = 300;

void render_dashboard(struct limine_framebuffer *fb, uint64_t frame) {
    uint32_t* pixels = fb->address;
    uint64_t width = fb->width;
    uint64_t height = fb->height;
    uint64_t pitch = fb->pitch / 4; 

    uint32_t tile_size = 32;

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            uint32_t color;
            if (x < 220) {
                color = 0xFF0F172A; 
                if (x >= 218) color = 0xFF3B82F6;
            } else {
                int checkered = ((x / tile_size) + (y / tile_size) + (frame / 20)) % 2;
                color = checkered ? 0xFF3B82F6 : 0xFFEF4444;
            }
            pixels[y * pitch + x] = color;
        }
    }

    int cw = 10, ch = 10;
    for (int cy = 0; cy < ch; cy++) {
        for (int cx = 0; cx < cw; cx++) {
            int px = cursor_x + cx;
            int py = cursor_y + cy;
            if (px >= 0 && px < (int)width && py >= 0 && py < (int)height) {
                if (cx == 0 || cx == cw - 1 || cy == 0 || cy == ch - 1) {
                    pixels[py * pitch + px] = 0xFFFF0000;
                } else {
                    pixels[py * pitch + px] = 0xFFFFFFFF;
                }
            }
        }
    }
}

void kernel_main(void) {
    kernel_print("\n=== TORUSOS KERNEL MAIN REACHED ===\n");

    simd_init();
    tier_scheduler_init();
    invariants_init();
    ps2_init();
    phasecore_interpreter_init();
    invariant_register(1, 2, "Active running tier must possess a valid state vector");

    // Test PhaseCore instruction interpretation from XML envelope using single quotes
    const char *instruction_envelope = "<instruction arg1='31' arg2='0'>TENSOR_MAP</instruction>";
    phasecore_interpret_envelope(instruction_envelope);

    if (framebuffer_request.response == NULL) {
        kernel_print("[Framebuffer] RESPONSE IS NULL!\n");
        for (;;) { __asm__ volatile("hlt"); }
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    uint64_t frame = 0;

    for (;;) {
        int32_t dx = 0, dy = 0;
        int left_click = 0;

        uint8_t status = inb(0x64);
        while (status & 1) {
            uint8_t data = inb(0x60);
            if (status & 0x20) {
                static uint8_t m_cycle = 0;
                static int8_t m_bytes[3];
                if (m_cycle == 0 && (data & 0x08)) {
                    m_bytes[0] = data;
                    m_cycle++;
                } else if (m_cycle == 1) {
                    m_bytes[1] = data;
                    m_cycle++;
                } else if (m_cycle == 2) {
                    m_bytes[2] = data;
                    m_cycle = 0;
                    dx += (int32_t)m_bytes[1] - (((m_bytes[0] & 0x10) << 4));
                    dy += -((int32_t)m_bytes[2] - (((m_bytes[0] & 0x20) << 3)));
                    left_click |= m_bytes[0] & 0x01;
                }
            } else {
                handle_keyboard_interrupt(data);
            }
            status = inb(0x64);
        }

        cursor_x += dx;
        cursor_y += dy;

        if (cursor_x < 0) cursor_x = 0;
        if (cursor_x >= (int32_t)fb->width) cursor_x = fb->width - 1;
        if (cursor_y < 0) cursor_y = 0;
        if (cursor_y >= (int32_t)fb->height) cursor_y = fb->height - 1;

        spatial_compositor_render();
        // render_dashboard(fb, frame); /* Handled by Spatial Compositor */
        invariants_evaluate_all();
        frame++;
        for (volatile int i = 0; i < 100000; i++);
    }
}
