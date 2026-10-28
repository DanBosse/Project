#ifndef PHASECORE_H
#define PHASECORE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TORUS_LOOP = 0x01,
    PHASE_SYNC = 0x02,
    MEM_SPLICE = 0x03,
    OPTIC_PULSE = 0x04,
    FLUX_READ = 0x05,
    ENTROPY_CLR = 0x06,
    TENSOR_MAP = 0x07,
    WAVE_INTERF = 0x08,
    MASS_FILTER = 0x09,
    STACK_FOLD = 0x0A,
    PIPE_STREAM = 0x0B,
    STATE_DECOMP = 0x0C,
    EXEC_HALT = 0x0D,
    CLOCK_PULSE = 0x0E,
    VECT_SHIFT = 0x0F,
    CORE_JUMP = 0x10
} PhaseCoreOpcode;

void phasecore_init(void);
void phasecore_dispatch(PhaseCoreOpcode op, uint64_t arg1, uint64_t arg2);
bool phasecore_run_validation_suite(void);

#endif
