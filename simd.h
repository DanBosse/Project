#ifndef SIMD_H
#define SIMD_H

#include <stddef.h>
#include <stdint.h>
#include "tensor.h"

void simd_init(void);
void* simd_allocate_context(void);
void simd_save(void* ctx);
void simd_restore(void* ctx);
void simd_tensor_accelerate(tensor_t *tensor, float scalar);

#endif
