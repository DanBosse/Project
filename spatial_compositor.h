#ifndef SPATIAL_COMPOSITOR_H
#define SPATIAL_COMPOSITOR_H

#include <stdint.h>

void spatial_compositor_init(void);
void spatial_compositor_render(uint64_t mb_info_addr);

#endif
