#ifndef DERIVED_PULLBACK_H
#define DERIVED_PULLBACK_H

#include <stdint.h>
#include <stdbool.h>

void derived_pullback_init(void);
uint64_t derived_pullback_map(uint64_t source_tensor_id, uint64_t target_tensor_id);
bool derived_pullback_verify_isomorphism(uint64_t mapping_handle);

#endif
