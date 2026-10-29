#include "twin_prime_alloc.h"
#include <stdint.h>
#include <stddef.h>

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

static uint64_t next_free_addr = 0x200000; // 2MB mark

void twin_prime_alloc_init(void) {
    kernel_print("[TwinPrimeAlloc] Initializing Layer 0 Twin Prime Memory Pools & Scheduling Spacing (Δ = 2)...\n");
}

uint64_t twin_prime_alloc_block(uint64_t size) {
    // Enforce twin-prime spacing invariant (delta = 2 alignment padding)
    uint64_t aligned_size = (size + 1) & ~1ULL;
    uint64_t allocated = next_free_addr;
    next_free_addr += aligned_size + 2; // Delta = 2 guard band
    return allocated;
}
