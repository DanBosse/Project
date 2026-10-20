#ifndef AURASCRIPT_WFS_H
#define AURASCRIPT_WFS_H

#include <stdint.h>

void aurascript_wfs_init(void);
int aurascript_register_spatial_source(const char* source_id, float x, float y, float z, float acoustical_mass);

#endif
