#ifndef AURASCRIPT_H
#define AURASCRIPT_H

#include <stdbool.h>

void aurascript_init(void);
bool aurascript_compile_stream(const char *source);
void aurascript_jit_execute(void);
void aurascript_dump_full_source(void);
void aurascript_eval(const char *arg);

#endif
