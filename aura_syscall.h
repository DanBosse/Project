#ifndef AURA_SYSCALL_H
#define AURA_SYSCALL_H

#include <stdint.h>
#include <stdbool.h>

#define SYS_NOP        0
#define SYS_PRINT      1
#define SYS_SOT_READ   2
#define SYS_TENSOR_MAP 3

void aura_syscall_init(void);
uint64_t aura_syscall_dispatch(uint32_t syscall_id, uint64_t arg1, uint64_t arg2);

#endif
