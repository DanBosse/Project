#ifndef ITERATIVE_REGULATOR_H
#define ITERATIVE_REGULATOR_H

#include <stdint.h>
#include <stdbool.h>

void iterative_regulator_init(void);
uint64_t iterative_normalize_flow(uint64_t thermal_state);
bool iterative_verify_equilibrium(void);

#endif
