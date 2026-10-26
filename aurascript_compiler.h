#ifndef AURASCRIPT_COMPILER_H
#define AURASCRIPT_COMPILER_H

#include <stdint.h>
#include <stddef.h>

// Kernel serial printing binding
void serial_print(const char* str);

// Core pipeline entry points
void cmd_aurascript(const char* arg);
void aurascript_dump_full_source(void);
void aurascript_parse_ast(const char* source);
void aurascript_jit_compile_node(const char* node_name);
void aurascript_vm_run(void);

#endif
