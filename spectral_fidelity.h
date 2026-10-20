#ifndef SPECTRAL_FIDELITY_H
#define SPECTRAL_FIDELITY_H

#include <stdint.h>
#include <stdbool.h>

void spectral_fidelity_init(void);
double spectral_compute_fidelity(const double *signal_vector, uint32_t length);
bool spectral_verify_fidelity_threshold(double i_eff, double threshold);

#endif
