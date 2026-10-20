#include "simd.h"
#include "mm.h"

extern void kernel_print(const char* str);

void simd_init(void) {
    uint64_t val;

    // Read and update CR0
    __asm__ volatile("movq %%cr0, %0" : "=r"(val));
    val &= ~(1UL << 2); // Clear EM (Emulation)
    val |= (1UL << 1);  // Set MP (Monitor Coprocessor)
    __asm__ volatile("movq %0, %%cr0" : : "r"(val) : "memory");

    // Read and update CR4
    __asm__ volatile("movq %%cr4, %0" : "=r"(val));
    val |= (1UL << 9);  // Set OSFXSR (FXSAVE/FXRSTOR support)
    val |= (1UL << 10); // Set OSXMMEXCPT (Unmasked SIMD FP exceptions)
    __asm__ volatile("movq %0, %%cr4" : : "r"(val) : "memory");

    kernel_print("[SIMD] AVX/SSE vector unit initialized in Ring 0.\n");
}

void* simd_allocate_context(void) {
    return kmalloc_aligned(512, 16);
}

void simd_save(void* ctx) {
    if (!ctx) return;
    __asm__ volatile("fxsave (%0)" : : "r"(ctx) : "memory");
}

void simd_restore(void* ctx) {
    if (!ctx) return;
    __asm__ volatile("fxrstor (%0)" : : "r"(ctx) : "memory");
}

void simd_tensor_accelerate(tensor_t *tensor, float scalar) {
    if (!tensor || !tensor->data) return;
    size_t total_elements = tensor->total_elements;
    float *data = tensor->data;

    for (size_t i = 0; i < total_elements; i++) {
        data[i] *= scalar;
    }
}
