#include <stdint.h>
#include <stddef.h>

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

struct limine_framebuffer_request {
    uint64_t id[4];
    uint64_t revision;
    struct limine_framebuffer_response *response;
};

extern struct limine_framebuffer_request framebuffer_request;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void serial_print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        outb(0x3F8, str[i]);
    }
}

void draw_splash_screen(void) {
    serial_print("[TorusOS] Entering draw_splash_screen...\n");

    if (framebuffer_request.response == NULL) {
        serial_print("[TorusOS ERROR] framebuffer_request.response is NULL!\n");
        return;
    }

    if (framebuffer_request.response->framebuffer_count == 0) {
        serial_print("[TorusOS ERROR] framebuffer_count is 0!\n");
        return;
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
    uint32_t *backbuffer = (uint32_t *)fb->address;

    if (backbuffer == NULL) {
        serial_print("[TorusOS ERROR] Framebuffer address is NULL!\n");
        return;
    }

    serial_print("[TorusOS] Framebuffer obtained successfully! Painting cyan...\n");

    uint64_t width = fb->width;
    uint64_t height = fb->height;
    uint64_t pitch = fb->pitch / 4;

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            backbuffer[y * pitch + x] = 0x0000FFFF; // Bright cyan
        }
    }
    serial_print("[TorusOS] Splash screen painted!\n");
}
