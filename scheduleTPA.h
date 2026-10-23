#ifndef TWIN_PRIME_ALLOC_H
#define TWIN_PRIME_ALLOC_H

#include <stdint.h>
#include <stdbool.h>

void scheduleTPA_init(void);
uint64_t scheduleTPA_alloc_block(uint64_t size);
bool scheduleTPA_free_block(uint64_t block_handle);

#endif
