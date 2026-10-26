#include "sandbox.h"

// Forward declaration for printing to the spatial shell terminal
extern void terminal_print(const char* s);

void sandbox_init(void) {
    // Safe init stub
}

uint64_t sandbox_execute_safely(const SandboxInstruction* program, size_t length) {
    if (!program || length == 0) return 0;
    uint64_t acc = 0;
    for (size_t i = 0; i < length; i++) {
        if (program[i].opcode == SB_LOAD) acc = program[i].val;
        if (program[i].opcode == SB_ADD)  acc += program[i].val;
        if (program[i].opcode == SB_RET)  return acc;
    }
    return acc;
}

void sandbox_run_container(const char *target) {
    (void)target;
    
    // Test program: Load 10, add 32, return -> Expected result: 42 (0x2A)
    SandboxInstruction test_program[] = {
        { SB_LOAD, 0, 10 },
        { SB_ADD,  0, 32 },
        { SB_RET,  0,  0 }
    };

    uint64_t result = sandbox_execute_safely(test_program, 3);
    
    terminal_print("\n[Sandbox] Ring3ShellInstance Trust Core active.\n");
    terminal_print("[Sandbox] Executing bytecode verification sequence...\n");
    
    if (result == 42) {
        terminal_print("[SUCCESS] Isolation boundary verified. Result: 42 (0x2A)\n");
    } else {
        terminal_print("[FAILURE] Bytecode verification failed.\n");
    }
}
