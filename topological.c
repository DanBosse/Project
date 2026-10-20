#include "topological.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void topological_init(void) {
    kernel_print("[Topological Engine] Initialized 16x master multiplier & manifold wave substrate.\n");
}

uint64_t topological_evaluate_frame(uint64_t tick, uint64_t F, uint64_t flex_sum, uint64_t M, uint64_t W_T) {
    if (M == 0) M = 1;
    uint64_t numerator = F + flex_sum;
    uint64_t division_result = numerator / M;
    uint64_t psi_reality = (16 * tick) * division_result * W_T;
    return psi_reality;
}

void topological_rasterize_frame(void *framebuffer, uint64_t width, uint64_t height, uint64_t pitch_bytes, uint64_t tick) {
    uint32_t *pixels = (uint32_t *)framebuffer;
    uint64_t pitch = pitch_bytes / 4;

    uint64_t F = 100;
    uint64_t flex_sum = 42 + (tick % 19);
    uint64_t M = 10;
    uint64_t W_T = 1;

    uint64_t psi = topological_evaluate_frame(tick, F, flex_sum, M, W_T);

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            uint32_t wave_val = (uint32_t)((x * y + tick + (psi & 0xFF)) % 256);
            
            uint32_t r = wave_val;
            uint32_t g = (wave_val + 64) % 256;
            uint32_t b = (wave_val + 128) % 256;

            if (x < 220) {
                pixels[y * pitch + x] = 0xFF0F172A;
            } else {
                pixels[y * pitch + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
            }
        }
    }
}
