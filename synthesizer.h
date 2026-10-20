#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <stdint.h>
#include <stdbool.h>

void synthesizer_init(void);
uint64_t synthesizer_generate_axiom(uint64_t input_genus);
bool synthesizer_verify_duality(uint64_t axiom_handle);

#endif
