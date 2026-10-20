#ifndef SANDBOX_H
#define SANDBOX_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Epistemic Sandbox Opcodes
typedef enum {
    SB_NOP   = 0x00,
    SB_LOAD  = 0x01, // Load register from scratchpad
    SB_STORE = 0x02, // Store register to scratchpad
    SB_ADD   = 0x03, // Add value to accumulator
    SB_CHECK = 0x04, // Verify invariant (G_chk)
    SB_RET   = 0x05  // Return execution result
} SandboxOpcode;

typedef struct {
    uint8_t opcode;
    uint8_t reg;
    uint16_t immediate;
} SandboxInstruction;

void sandbox_init(void);
bool sandbox_verify(const SandboxInstruction* program, size_t length);
uint64_t sandbox_execute_safely(const SandboxInstruction* program, size_t length);

#endif
