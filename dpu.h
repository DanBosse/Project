#ifndef DPU_H
#define DPU_H

#include <stdint.h>
#include <stdbool.h>

void dpu_init(void);
bool dpu_accelerate_stream(const char *stream_name, uint32_t bytes);

#endif
