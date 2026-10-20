#ifndef FLOWSCRIPT_H
#define FLOWSCRIPT_H

#include <stdint.h>

void flowscript_init(void);
int flowscript_compile_and_execute(const char *stream);
int flowscript_execute_line(const char *line);

#endif
