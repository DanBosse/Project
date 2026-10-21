#ifndef PHYSICS_LAB_H
#define PHYSICS_LAB_H

#include <stdint.h>

void physics_lab_init(void);
void physics_lab_render(void);
void physics_lab_step(void);
void physics_lab_reset(void);
void physics_lab_set_stiffness(float k);

#endif
