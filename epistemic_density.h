#ifndef EPISTEMIC_DENSITY_H
#define EPISTEMIC_DENSITY_H

#include <stdint.h>
#include <stdbool.h>

#define DENSITY_DIMENSION 8

typedef struct {
    double real[DENSITY_DIMENSION][DENSITY_DIMENSION];
    double imag[DENSITY_DIMENSION][DENSITY_DIMENSION];
} EpistemicDensityMatrix;

void epistemic_density_init(void);
void epistemic_update_state(EpistemicDensityMatrix *rho, uint64_t tick);
double epistemic_calculate_trace(const EpistemicDensityMatrix *rho);
bool epistemic_verify_governance(const EpistemicDensityMatrix *rho);

#endif
