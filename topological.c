#include "topological.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void topological_init(void) {
    kernel_print("[Topological Engine] Initialized 3D Raymarched Rotating Manifold Sphere.\n");
}

uint64_t topological_evaluate_frame(uint64_t tick, uint64_t F, uint64_t flex_sum, uint64_t M, uint64_t W_T) {
    if (M == 0) M = 1;
    uint64_t numerator = F + flex_sum;
    uint64_t division_result = numerator / M;
    uint64_t psi_reality = (16 * tick) * division_result * W_T;
    return psi_reality;
}

static float kernel_sqrt(float val) {
    if (val <= 0.0f) return 0.0f;
    float x = val * 0.5f;
    for (int i = 0; i < 4; i++) {
        x = 0.5f * (x + val / x);
    }
    return x;
}

static float kernel_sin(float x) {
    while (x > 3.14159265f) x -= 6.28318531f;
    while (x < -3.14159265f) x += 6.28318531f;
    float num = 4.0f * x * (3.14159265f - (x < 0 ? -x : x));
    float den = 12.7323954f - (x < 0 ? -x : x) * (3.14159265f - (x < 0 ? -x : x));
    if (den == 0.0f) return 0.0f;
    return num / den;
}

static float kernel_cos(float x) {
    return kernel_sin(x + 1.57079632f);
}

static float kernel_asin_approx(float x) {
    if (x > 1.0f) x = 1.0f;
    if (x < -1.0f) x = -1.0f;
    return x * (1.57079632f - 0.62831853f * (x < 0 ? -x : x) + 1.57079632f * x * x);
}

static float kernel_atan2_approx(float y, float x) {
    if (x == 0.0f) {
        return y > 0.0f ? 1.57079632f : -1.57079632f;
    }
    float z = y / x;
    float atan = z / (1.0f + 0.28f * z * z);
    if (x < 0.0f) {
        return y >= 0.0f ? atan + 3.14159265f : atan - 3.14159265f;
    }
    return atan;
}

void topological_rasterize_frame(void *framebuffer, uint64_t width, uint64_t height, uint64_t pitch_bytes, uint64_t tick) {
    uint32_t *pixels = (uint32_t *)framebuffer;
    uint64_t pitch = pitch_bytes / 4;

    float cx = 434.0f; 
    float cy = (float)height * 0.5f;
    float t = (float)tick * 0.04f;
    float sphere_radius = 160.0f;

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            if (x < 220) {
                pixels[y * pitch + x] = 0xFF0F172A;
                continue;
            }

            float fx = (float)x - cx;
            float fy = (float)y - cy;
            float r2 = fx * fx + fy * fy;

            uint32_t r, g, b;

            if (r2 < sphere_radius * sphere_radius) {
                float fz = kernel_sqrt(sphere_radius * sphere_radius - r2);
                
                float nx = fx / sphere_radius;
                float ny = fy / sphere_radius;
                float nz = fz / sphere_radius;

                float cos_t = kernel_cos(t);
                float sin_t = kernel_sin(t);
                float rot_x = nx * cos_t - nz * sin_t;
                float rot_z = nx * sin_t + nz * cos_t;

                float lat = kernel_asin_approx(ny);
                float lon = kernel_atan2_approx(rot_z, rot_x) + t;
                
                float pattern = kernel_sin(lon * 8.0f) * kernel_cos(lat * 8.0f);
                float ndotl = rot_x * (-0.577f) + ny * (-0.577f) + rot_z * (0.577f);
                if (ndotl < 0.1f) ndotl = 0.1f;

                r = (uint32_t)(30.0f + pattern * 40.0f + ndotl * 120.0f);
                g = (uint32_t)(80.0f + pattern * 60.0f + ndotl * 150.0f);
                b = (uint32_t)(160.0f + pattern * 80.0f + ndotl * 95.0f);
            } else {
                float dist = kernel_sqrt(r2);
                float v1 = kernel_sin(fx * 0.02f + t) * kernel_cos(fy * 0.02f - t);
                float field = (v1 + 1.0f) * 45.0f;

                r = (uint32_t)(15.0f + field * 0.4f);
                g = (uint32_t)(25.0f + field * 0.7f);
                b = (uint32_t)(50.0f + field * 1.1f);

                if ((x % 48 == 0) || (y % 48 == 0)) {
                    r += 20;
                    g += 35;
                    b += 60;
                }
            }

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            pixels[y * pitch + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }
}
