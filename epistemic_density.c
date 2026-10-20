#include "epistemic_density.h"

extern void kernel_print(const char* str);

void epistemic_density_init(void) {
    kernel_print("[EpistemicDensity] Initializing continuous space density operator (^rho_epistemic)...\n");
}

double epistemic_calculate_trace(const EpistemicDensityMatrix *rho) {
    double trace_real = 0.0;
    for (int i = 0; i < DENSITY_DIMENSION; i++) {
        trace_real += rho->real[i][i];
    }
    return trace_real;
}

void epistemic_update_state(EpistemicDensityMatrix *rho, uint64_t tick) {
    double normalization = 0.0;
    for (int i = 0; i < DENSITY_DIMENSION; i++) {
        rho->real[i][i] = 1.0 / (double)DENSITY_DIMENSION + (0.01 * (i == (tick % DENSITY_DIMENSION) ? 1.0 : -0.1));
        normalization += rho->real[i][i];
    }
    for (int i = 0; i < DENSITY_DIMENSION; i++) {
        rho->real[i][i] /= normalization;
    }
}

bool epistemic_verify_governance(const EpistemicDensityMatrix *rho) {
    double trace = epistemic_calculate_trace(rho);
    double diff = trace - 1.0;
    if (diff < 0) diff = -diff;
    return diff < 0.0001;
}
