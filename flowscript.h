#ifndef FLOWSCRIPT_H
#define FLOWSCRIPT_H

#include <stdint.h>
#include <stdbool.h>

void flowscript_init(void);
bool flowscript_execute(const char *source_code);
bool flowscript_execute_line(const char *source_code);

#endif
