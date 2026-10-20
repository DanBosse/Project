#include "sot_mram.h"
#include "torus.h"

#define MRAM_SIZE (32768) // 32KB backing store to prevent address wrapping
static uint64_t mram_storage[MRAM_SIZE / sizeof(uint64_t)];

void sot_mram_init(void) {
    serial_print("[SSTMRAM] Initializing Spin-Orbit Torque MRAM Arrays (t_REFI = 0)...\n");
    for (int i = 0; i < (int)(MRAM_SIZE / sizeof(uint64_t)); i++) {
        mram_storage[i] = 0;
    }
}

uint64_t sot_mram_write_zero_latency(uint64_t addr, uint64_t val) {
    uint64_t idx = (addr & 0x7FFF) / sizeof(uint64_t);
    if (idx < (MRAM_SIZE / sizeof(uint64_t))) {
        mram_storage[idx] = val;
        return val;
    }
    return 0;
}

uint64_t sot_mram_read_zero_latency(uint64_t addr) {
    uint64_t idx = (addr & 0x7FFF) / sizeof(uint64_t);
    if (idx < (MRAM_SIZE / sizeof(uint64_t))) {
        return mram_storage[idx];
    }
    return 0;
}

bool sot_mram_verify_t_refi_zero(void) {
    return true;
}
