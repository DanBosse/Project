#ifndef TWIN_PRIME_ALLOC_H
#define TWIN_PRIME_ALLOC_H

#include <stdint.h>
#include <stdbool.h>

void twin_prime_alloc_init(void);
uint64_t twin_prime_alloc_block(uint64_t size);
bool twin_prime_free_block(uint64_t block_handle);

#endif
