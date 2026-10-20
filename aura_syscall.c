#include "aura_syscall.h"
#include <stdint.h>
#include <stdbool.h>

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

// External FFI link to Trust Core syscall verifier
extern bool trust_syscall_verify(uint32_t syscall_id, uint64_t arg1);

void aura_syscall_init(void) {
    kernel_print("[AuraSyscall] Initializing Ring 3-to-Ring 0 Sovereign Syscall Gateway with Trust Core...\n");
}

uint64_t aura_syscall_dispatch(uint32_t syscall_id, uint64_t arg1, uint64_t arg2) {
    (void)arg2;
    
    // Enforce Trust Core verification before executing syscall
    if (!trust_syscall_verify(syscall_id, arg1)) {
        kernel_print("[AuraSyscall] SECURITY ALERT: Invalid syscall or argument blocked by Trust Core!\n");
        return 0xFFFFFFFFFFFFFFFFULL; // Error sentinel
    }

    switch (syscall_id) {
        case SYS_NOP:
            return 0;
        case SYS_PRINT:
            kernel_print("[AuraSyscall] Ring 3 Print Request Executed.\n");
            return 0;
        case SYS_SOT_READ:
            return 0x4f544dd2414d00ULL; // SOT-MRAM value
        case SYS_TENSOR_MAP:
            kernel_print("[AuraSyscall] Verified Tensor Map request for index: ");
            kernel_print_hex(arg1);
            kernel_print("\n");
            return arg1 * 2;
        default:
            return 0;
    }
}
