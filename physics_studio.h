#ifndef PHYSICS_STUDIO_H
#define PHYSICS_STUDIO_H

#include <stdint.h>
#include <stdbool.h>

void physics_studio_init(void);
bool auraspring_rt_run(uint32_t bodies, uint32_t sub_steps);
bool auraspring_xr_run(const char *manifold_model, uint32_t dimensions);
void physics_studio_full_pipeline(void);

#endif
