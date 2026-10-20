#ifndef TOPOLOGICAL_H
#define TOPOLOGICAL_H

#include <stdint.h>

void topological_init(void);
uint64_t topological_evaluate_frame(uint64_t tick, uint64_t F, uint64_t flex_sum, uint64_t M, uint64_t W_T);
void topological_rasterize_frame(void *framebuffer, uint64_t width, uint64_t height, uint64_t pitch, uint64_t tick);

#endif
