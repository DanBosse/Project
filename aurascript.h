#ifndef AURASCRIPT_H
#define AURASCRIPT_H

#include <stdint.h>

void aurascript_init(void);
int aurascript_compile_master(void);
void aurascript_eval(const char* expr);
void aurascript_set_variable(const char* name, int value);
int aurascript_get_variable(const char* name);

#endif // AURASCRIPT_H
void aurascript_launch_ring3_container(void);
