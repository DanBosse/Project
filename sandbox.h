#ifndef SANDBOX_H
#define SANDBOX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    SB_LOAD = 0,
    SB_ADD  = 1,
    SB_RET  = 2
} SandboxOpcode;

typedef struct {
    uint8_t opcode;
    uint8_t reg;
    uint64_t val;
} SandboxInstruction;

void sandbox_init(void);
uint64_t sandbox_execute_safely(const SandboxInstruction* program, size_t length);
void sandbox_run_container(const char *target);

#endif
