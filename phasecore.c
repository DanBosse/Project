#include "phasecore.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void phasecore_dispatch(PhaseCoreOpcode op, uint64_t arg1, uint64_t arg2) {
    (void)arg1;
    (void)arg2;

    kernel_print("[PhaseCore] Executing Opcode: ");
    kernel_print_hex((uint64_t)op);
    kernel_print("\n");

    switch(op) {
        case TORUS_LOOP:
            break;
        case PHASE_SYNC:
            __asm__ volatile("nop");
            break;
        case MEM_SPLICE:
            break;
        case OPTIC_PULSE:
            break;
        case FLUX_READ:
            break;
        case ENTROPY_CLR:
            break;
        case TENSOR_MAP:
            break;
        case WAVE_INTERF:
            break;
        case MASS_FILTER:
            break;
        case STACK_FOLD:
            break;
        case PIPE_STREAM:
            break;
        case STATE_DECOMP:
            break;
        case EXEC_HALT:
            for (;;) { __asm__ volatile("hlt"); }
            break;
        case CLOCK_PULSE:
            break;
        case VECT_SHIFT:
            break;
        case CORE_JUMP:
            break;
        default:
            kernel_print("[PhaseCore] Unknown Opcode!\n");
            break;
    }
}
