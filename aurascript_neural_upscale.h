#ifndef AURASCRIPT_NEURAL_UPSCALE_H
#define AURASCRIPT_NEURAL_UPSCALE_H

#include <stdint.h>
#include <stdbool.h>

void aurascript_neural_upscale_init(void);
int aurascript_configure_super_resolution(uint32_t scale_factor, bool tensor_acceleration);

#endif
