#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TENSOR_DIMS 4

typedef struct {
    uint32_t num_dims;
    uint32_t shape[MAX_TENSOR_DIMS];
    uint32_t strides[MAX_TENSOR_DIMS];
    size_t total_elements;
    float *data;
} tensor_t;

tensor_t* tensor_create(uint32_t num_dims, const uint32_t* shape);
void tensor_free(tensor_t* tensor);
void tensor_fill(tensor_t* t, float val);
int tensor_add(const tensor_t* a, const tensor_t* b, tensor_t* dest);
float tensor_dot_product(const tensor_t* a, const tensor_t* b);
void tensor_scale(tensor_t* t, float scalar);

#endif
