#ifndef COLLAPSE_BRIDGE_H
#define COLLAPSE_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>

void collapse_bridge_init(void);
uint64_t collapse_execute_translation(uint64_t proof_id);
bool collapse_verify_predicate(uint64_t proof_handle, uint64_t prime_checksum);

#endif
