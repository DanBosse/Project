#include "tensor.h"
#include <stddef.h>

// External aligned allocator helper (e.g., linked to PMM or heap)
extern void* kmalloc_aligned(size_t size, size_t alignment);
extern void kfree(void* ptr);

tensor_t* tensor_create(uint32_t num_dims, const uint32_t* shape) {
    if (num_dims == 0 || num_dims > MAX_TENSOR_DIMS) {
        return NULL;
    }

    tensor_t* t = (tensor_t*)kmalloc_aligned(sizeof(tensor_t), 16);
    if (!t) return NULL;

    t->num_dims = num_dims;
    size_t elements = 1;

    for (int i = (int)num_dims - 1; i >= 0; i--) {
        t->shape[i] = shape[i];
        t->strides[i] = (uint32_t)elements;
        elements *= shape[i];
    }
    t->total_elements = elements;

    // Allocate 64-byte aligned data buffer for optimal SIMD / AVX loads
    size_t byte_size = elements * sizeof(float);
    t->data = (float*)kmalloc_aligned(byte_size, 64);
    if (!t->data) {
        kfree(t);
        return NULL;
    }

    return t;
}

void tensor_free(tensor_t* t) {
    if (t) {
        if (t->data) {
            kfree(t->data);
        }
        kfree(t);
    }
}

void tensor_fill(tensor_t* t, float val) {
    if (!t || !t->data) return;
    for (size_t i = 0; i < t->total_elements; i++) {
        t->data[i] = val;
    }
}

int tensor_add(const tensor_t* a, const tensor_t* b, tensor_t* dest) {
    if (!a || !b || !dest) return -1;
    if (a->total_elements != b->total_elements || a->total_elements != dest->total_elements) {
        return -1; // Shape mismatch
    }

    // Unrolled loop for baseline kernel performance (can be replaced with AVX intrinsics)
    size_t n = a->total_elements;
    size_t i = 0;

    for (; i + 3 < n; i += 4) {
        dest->data[i]     = a->data[i]     + b->data[i];
        dest->data[i + 1] = a->data[i + 1] + b->data[i + 1];
        dest->data[i + 2] = a->data[i + 2] + b->data[i + 2];
        dest->data[i + 3] = a->data[i + 3] + b->data[i + 3];
    }
    for (; i < n; i++) {
        dest->data[i] = a->data[i] + b->data[i];
    }

    return 0;
}

float tensor_dot_product(const tensor_t* a, const tensor_t* b) {
    if (!a || !b || a->total_elements != b->total_elements) {
        return 0.0f;
    }

    float sum = 0.0f;
    size_t n = a->total_elements;
    for (size_t i = 0; i < n; i++) {
        sum += a->data[i] * b->data[i];
    }
    return sum;
}

void tensor_scale(tensor_t* t, float scalar) {
    if (!t || !t->data) return;
    for (size_t i = 0; i < t->total_elements; i++) {
        t->data[i] *= scalar;
    }
}
