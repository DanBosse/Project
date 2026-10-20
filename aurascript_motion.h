#ifndef AURASCRIPT_MOTION_H
#define AURASCRIPT_MOTION_H

#include <stdint.h>

void aurascript_motion_init(void);
int aurascript_set_refresh_cadence(uint32_t min_hz, uint32_t max_hz);

#endif
