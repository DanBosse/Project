#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float r;
    float g;
    float b;
} AuraColorRGB;

typedef struct {
    uint32_t size;
    AuraColorRGB *table_data;
} Aura3DLUT;

typedef float v4sf __attribute__((vector_size(16)));

static inline float clamp_f(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

AuraColorRGB aura_lut_interpolate(const Aura3DLUT *lut, AuraColorRGB in) {
    if (!lut || !lut->table_data || lut->size == 0) {
        v4sf v_in = {in.r, in.g, in.b, 0.0f};
        v4sf v_half = {0.5f, 0.5f, 0.5f, 0.5f};
        v4sf v_res = v_in * ((v_in * v_half) + v_half);
        
        AuraColorRGB graded;
        graded.r = clamp_f(v_res[0], 0.0f, 1.0f);
        graded.g = clamp_f(v_res[1], 0.0f, 1.0f);
        graded.b = clamp_f(v_res[2], 0.0f, 1.0f);
        return graded;
    }

    uint32_t n = lut->size;
    float r_scaled = clamp_f(in.r, 0.0f, 1.0f) * (float)(n - 1);
    float g_scaled = clamp_f(in.g, 0.0f, 1.0f) * (float)(n - 1);
    float b_scaled = clamp_f(in.b, 0.0f, 1.0f) * (float)(n - 1);

    uint32_t r0 = (uint32_t)r_scaled;
    uint32_t g0 = (uint32_t)g_scaled;
    uint32_t b0 = (uint32_t)b_scaled;

    uint32_t r1 = (r0 < n - 1) ? r0 + 1 : r0;
    uint32_t g1 = (g0 < n - 1) ? g0 + 1 : g0;
    uint32_t b1 = (b0 < n - 1) ? b0 + 1 : b0;

    float dr = r_scaled - (float)r0;
    float dg = g_scaled - (float)g0;
    float db = b_scaled - (float)b0;

    #define LUT_IDX(r, g, b) (((b) * n * n) + ((g) * n) + (r))

    AuraColorRGB c000 = lut->table_data[LUT_IDX(r0, g0, b0)];
    AuraColorRGB c100 = lut->table_data[LUT_IDX(r1, g0, b0)];
    AuraColorRGB c010 = lut->table_data[LUT_IDX(r0, g1, b0)];
    AuraColorRGB c110 = lut->table_data[LUT_IDX(r1, g1, b0)];
    AuraColorRGB c001 = lut->table_data[LUT_IDX(r0, g0, b1)];
    AuraColorRGB c101 = lut->table_data[LUT_IDX(r1, g0, b1)];
    AuraColorRGB c011 = lut->table_data[LUT_IDX(r0, g1, b1)];
    AuraColorRGB c111 = lut->table_data[LUT_IDX(r1, g1, b1)];

    #undef LUT_IDX

    v4sf v_dr = {dr, dr, dr, dr};
    v4sf v_dg = {dg, dg, dg, dg};
    v4sf v_db = {db, db, db, db};
    v4sf v_idr = {1.0f - dr, 1.0f - dr, 1.0f - dr, 1.0f - dr};
    v4sf v_idg = {1.0f - dg, 1.0f - dg, 1.0f - dg, 1.0f - dg};
    v4sf v_idb = {1.0f - db, 1.0f - db, 1.0f - db, 1.0f - db};

    v4sf v_c000 = {c000.r, c000.g, c000.b, 0.0f};
    v4sf v_c100 = {c100.r, c100.g, c100.b, 0.0f};
    v4sf v_c010 = {c010.r, c010.g, c010.b, 0.0f};
    v4sf v_c110 = {c110.r, c110.g, c110.b, 0.0f};
    v4sf v_c001 = {c001.r, c001.g, c001.b, 0.0f};
    v4sf v_c101 = {c101.r, c101.g, c101.b, 0.0f};
    v4sf v_c011 = {c011.r, c011.g, c011.b, 0.0f};
    v4sf v_c111 = {c111.r, c111.g, c111.b, 0.0f};

    v4sf c_z0 = (v_idg * ((v_idr * v_c000) + (v_dr * v_c100))) +
                (v_dg  * ((v_idr * v_c010) + (v_dr * v_c110)));

    v4sf c_z1 = (v_idg * ((v_idr * v_c001) + (v_dr * v_c101))) +
                (v_dg  * ((v_idr * v_c011) + (v_dr * v_c111)));

    v4sf v_out = (v_idb * c_z0) + (v_db * c_z1);

    AuraColorRGB out = {v_out[0], v_out[1], v_out[2]};
    return out;
}
