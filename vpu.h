#ifndef VPU_H
#define VPU_H

#include <stdint.h>
#include <stdbool.h>

void vpu_init(void);
bool vpu_accelerate_render(const char *workload_type, uint32_t resolution_width, uint32_t resolution_height);

#endif
