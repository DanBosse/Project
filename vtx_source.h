#ifndef VTX_SOURCE_H
#define VTX_SOURCE_H

#include <stdint.h>

void vtx_source_init(void);
int vtx_compile_and_execute(const char* vtx_program);

#endif
