#include "sandbox.h"
#include <stdint.h>
#include <stdbool.h>

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

// External FFI link to Trust Core verifier
extern bool trust_sandbox_verify(const SandboxInstruction* program, size_t length);

#define SCRATCHPAD_SIZE 16
static uint64_t scratchpad[SCRATCHPAD_SIZE];

void sandbox_init(void) {
    kernel_print("[Sandbox] Initializing Ring 0 Epistemic Sandbox (Trust Core Verification)...\n");
    for (int i = 0; i < SCRATCHPAD_SIZE; i++) {
        scratchpad[i] = i * 10;
    }
}

bool sandbox_verify(const SandboxInstruction* program, size_t length) {
    // Delegate static verification to the Trust Core
    return trust_sandbox_verify(program, length);
}

uint64_t sandbox_execute_safely(const SandboxInstruction* program, size_t length) {
    if (!sandbox_verify(program, length)) {
        kernel_print("[Sandbox] Execution aborted by Trust Core (safety violation caught).\n");
        return 0;
    }

    uint64_t accumulator = 0;
    uint32_t instruction_budget = 1000;

    for (size_t pc = 0; pc < length && instruction_budget > 0; pc++, instruction_budget--) {
        SandboxInstruction ins = program[pc];
        switch (ins.opcode) {
            case SB_NOP:
                break;
            case SB_LOAD:
                accumulator = scratchpad[ins.reg];
                break;
            case SB_STORE:
                scratchpad[ins.reg] = accumulator;
                break;
            case SB_ADD:
                accumulator += ins.immediate;
                break;
            case SB_CHECK:
                if (accumulator == 0) {
                    return 0;
                }
                break;
            case SB_RET:
                return accumulator;
        }
    }

    return accumulator;
}
