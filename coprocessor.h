#ifndef COPROCESSOR_H
#define COPROCESSOR_H
#include <stdbool.h>
void coprocessor_init(void);
bool coprocessor_dispatch_tensor(const char *task_name);
bool aurapu_npu_inference(const char *model_type, const float *input_tensor, int size);
#endif
