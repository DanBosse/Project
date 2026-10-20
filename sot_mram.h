#ifndef SOT_MRAM_H
#define SOT_MRAM_H

#include <stdint.h>
#include <stdbool.h>

void sot_mram_init(void);
uint64_t sot_mram_write_zero_latency(uint64_t addr, uint64_t data);
uint64_t sot_mram_read_zero_latency(uint64_t addr);
bool sot_mram_verify_t_refi_zero(void);

#endif
