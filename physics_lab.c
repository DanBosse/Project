#include "studio_core.h"
#include "physics_lab.h"
#include "torus.h"
#include "limine/limine.h"
#include <stddef.h>
#include <stdint.h>

extern volatile struct limine_framebuffer_request framebuffer_request;

#define WIDTH 1024
#define HEIGHT 768

static float spring_position = 200.0f;
static float spring_velocity = 0.0f;
static float target_position = 500.0f;
static float mass = 1.0f;
static float stiffness = 35.0f;
static float damping = 3.5f;

void physics_lab_init(void) {
    serial_print("[PhysicsLab] Initializing AuraSpring Advanced Simulation Lab...\n");
    physics_lab_reset();
}

void physics_lab_reset(void) {
    spring_position = 200.0f;
    spring_velocity = 0.0f;
}

void physics_lab_set_stiffness(float k) {
    stiffness = k;
}

void physics_lab_step(void) {
    float pipe_data[2] = { spring_position, spring_velocity };
    studio_core_write_pipe(0, pipe_data, 2, "PhysicsSpring");

    float displacement = spring_position - target_position;
    float force = -(stiffness * displacement) - (damping * spring_velocity);
    float acceleration = force / mass;
    spring_velocity += acceleration * 0.1f;
    spring_position += spring_velocity * 0.1f;
}

static void draw_rect(uint32_t *fb, uint32_t w, uint32_t h, uint32_t rx, uint32_t ry, uint32_t rw, uint32_t rh, uint32_t color) {
    for (uint32_t y = ry; y < ry + rh && y < h; y++) {
        for (uint32_t x = rx; x < rx + rw && x < w; x++) {
            fb[y * w + x] = color;
        }
    }
}

void physics_lab_render(void) {
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

    for (int frame = 0; frame < 90; frame++) {
        physics_lab_step();

        // Background
        draw_rect(fb, width, height, 0, 0, width, height, 0x00090D16);

        // StudioLens Header for Physics Lab
        draw_rect(fb, width, height, 30, 20, width - 60, 60, 0x001E293B);
        
        // Lab Workspace Panels
        draw_rect(fb, width, height, 30, 100, width - 60, height - 130, 0x000F172A);

        // Spring Anchor & Mass
        draw_rect(fb, width, height, 180, 260, 20, 200, 0x00334155);
        uint32_t node_x = (uint32_t)spring_position;
        if (node_x < 200) node_x = 200;
        if (node_x > width - 140) node_x = width - 140;

        for (uint32_t x = 200; x < node_x; x += 15) {
            draw_rect(fb, width, height, x, 350, 8, 12, 0x0038BDF8);
        }

        draw_rect(fb, width, height, node_x, 290, 100, 140, 0x0038BDF8);

        for (volatile int d = 0; d < 6000000; d++);
    }
    serial_print("[PhysicsLab] StudioLens Physics simulation completed.\n");
}
