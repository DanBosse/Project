#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>
#include <stdbool.h>

void benchmark_init(void);
void benchmark_run_mram_saturation(uint32_t iterations);
void benchmark_run_twinalloc_stress(uint32_t iterations);

#endif
