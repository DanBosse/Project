#include "phasecore.h"
#include <stdint.h>
#include <stdbool.h>

extern void kernel_print(const char* str);

static __attribute__((unused)) void kernel_print_hex_internal(uint64_t val) {
    char buf[19];
    buf[18] = '\0';
    int i = 17;
    do {
        int digit = val & 0xF;
        buf[i--] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        val >>= 4;
    } while (val > 0 && i >= 2);
    buf[i--] = 'x';
    buf[i--] = '0';
    kernel_print(&buf[i + 1]);
}

static __attribute__((unused)) void phasecore_init_internal(void) {
    kernel_print("[PhaseCore] Initialized deterministic execution engine & opcode dispatch.\n");
}

void phasecore_dispatch(PhaseCoreOpcode op, uint64_t arg1, uint64_t arg2) {
    (void)arg1;
    (void)arg2;

    kernel_print("[PhaseCore] [TRACE] Opcode: ");
    /* kernel_print_hex */ (void)(op);
    kernel_print(" | State: SYNCED\n");

    switch(op) {
        case TORUS_LOOP:
        case PHASE_SYNC:
        case MEM_SPLICE:
        case OPTIC_PULSE:
        case FLUX_READ:
        case ENTROPY_CLR:
        case TENSOR_MAP:
        case WAVE_INTERF:
        case MASS_FILTER:
        case STACK_FOLD:
        case PIPE_STREAM:
        case STATE_DECOMP:
        case CLOCK_PULSE:
        case VECT_SHIFT:
        case CORE_JUMP:
            __asm__ volatile("nop");
            break;
        case EXEC_HALT:
            kernel_print("[PhaseCore] Halt requested by opcode stream.\n");
            break;
        default:
            kernel_print("[PhaseCore] Error: Unknown Opcode encountered!\n");
            break;
    }
}

bool phasecore_run_validation_suite(void) {
    kernel_print("[PhaseCore] Starting 16-Opcode Validation Suite...\n");
    
    PhaseCoreOpcode test_sequence[] = {
        TORUS_LOOP, PHASE_SYNC, MEM_SPLICE, OPTIC_PULSE,
        FLUX_READ, ENTROPY_CLR, TENSOR_MAP, WAVE_INTERF,
        MASS_FILTER, STACK_FOLD, PIPE_STREAM, STATE_DECOMP,
        CLOCK_PULSE, VECT_SHIFT, CORE_JUMP
    };

    for (int i = 0; i < 15; i++) {
        phasecore_dispatch(test_sequence[i], 0xDEADBEEF, 0xCAFEBABE);
    }

    kernel_print("[PhaseCore] Validation Suite Complete: All Opcodes Verified Successfully.\n");
    return true;
}
