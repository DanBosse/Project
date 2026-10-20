#ifndef EPISTEMIC_COPROCESSOR_H
#define EPISTEMIC_COPROCESSOR_H

#include <stdint.h>
#include <stdbool.h>

void epistemic_coprocessor_init(void);
uint64_t epistemic_route_trace(uint64_t process_id, double density_value);
bool epistemic_verify_trace_governance(uint64_t trace_handle);

#endif
