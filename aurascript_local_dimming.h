#ifndef AURASCRIPT_LOCAL_DIMMING_H
#define AURASCRIPT_LOCAL_DIMMING_H

#include <stdint.h>
#include <stdbool.h>

void aurascript_local_dimming_init(void);
int aurascript_set_dimming_zones(uint32_t grid_columns, uint32_t grid_rows, bool suppression_active);

#endif
