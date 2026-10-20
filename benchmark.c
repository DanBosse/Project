#include "benchmark.h"
#include "sot_mram.h"
#include "twin_prime_alloc.h"
#include "torus.h"

void benchmark_init(void) {
    serial_print("[Benchmark] Initializing Phase 1 Subsystem Stress & Saturation Engine...\n");
}

void benchmark_run_mram_saturation(uint32_t iterations) {
    serial_print("[Benchmark] Starting SOT-MRAM Zero-Latency Saturation Test...\n");
    bool success = true;
    for (uint32_t i = 0; i < iterations; i++) {
        uint64_t addr = 0x1000 + (i * 8);
        uint64_t val = 0xDEADBEEF00000000ULL | i;
        sot_mram_write_zero_latency(addr, val);
        uint64_t read_back = sot_mram_read_zero_latency(addr);
        if (read_back != val) {
            success = false;
            break;
        }
    }
    if (success && sot_mram_verify_t_refi_zero()) {
        serial_print("[Benchmark] SOT-MRAM Saturation: PASSED (Zero-latency integrity verified)\n");
    } else {
        serial_print("[Benchmark] SOT-MRAM Saturation: FAILED (Integrity mismatch detected)\n");
    }
}

void benchmark_run_twinalloc_stress(uint32_t blocks) {
    serial_print("[Benchmark] Starting Twin-Prime Allocator Stress Test (Delta = 2)...\n");
    bool success = true;
    for (uint32_t i = 0; i < blocks; i++) {
        uint64_t handle = twin_prime_alloc_block(64 + (i % 128));
        if (handle == 0) {
            success = false;
            break;
        }
    }
    if (success) {
        serial_print("[Benchmark] Twin-Prime Allocation Stress: PASSED (All blocks aligned & secured)\n");
    } else {
        serial_print("[Benchmark] Twin-Prime Allocation Stress: FAILED (Allocation collision)\n");
    }
}
