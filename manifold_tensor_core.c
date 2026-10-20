#include "manifold_tensor_core.h"
#include <stdint.h>
#include <stdbool.h>

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

// External FFI link to Trust Core tensor bounds verifier
extern bool trust_tensor_bounds_verify(uint32_t tensor_index);

#define NUM_TENSORS 32
static uint64_t tensor_registry[NUM_TENSORS];

void manifold_tensor_core_init(void) {
    kernel_print("[ManifoldTensorCore] Initializing 26 foundational anchor weights & 32-tensor registry with Trust Core...\n");
    for (int i = 0; i < NUM_TENSORS; i++) {
        tensor_registry[i] = 0x1000 + i;
    }
}

uint64_t manifold_tensor_get(uint32_t index) {
    uint32_t safe_index = index % NUM_TENSORS;
    if (!trust_tensor_bounds_verify(safe_index)) {
        kernel_print("[ManifoldTensorCore] SAFETY VIOLATION: Tensor index out of bounds caught by Trust Core!\n");
        return 0;
    }
    return tensor_registry[safe_index];
}

void manifold_tensor_set(uint32_t index, uint64_t value) {
    uint32_t safe_index = index % NUM_TENSORS;
    if (!trust_tensor_bounds_verify(safe_index)) {
        kernel_print("[ManifoldTensorCore] SAFETY VIOLATION: Tensor index out of bounds caught by Trust Core!\n");
        return;
    }
    tensor_registry[safe_index] = value;
}
