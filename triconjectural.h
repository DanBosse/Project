#ifndef TRICONJECTURAL_H
#define TRICONJECTURAL_H

#include <stdint.h>
#include <stdbool.h>

void triconjectural_init(void);
bool triconjectural_verify_anchor(uint64_t tensor_handle, uint64_t truth_index);
uint64_t triconjectural_dampen_flux(uint64_t manifold_pressure);
bool triconjectural_validate_twin_spacing(uint64_t thread_addr_a, uint64_t thread_addr_b);

#endif
