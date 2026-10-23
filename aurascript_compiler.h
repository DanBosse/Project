#ifndef AURASCRIPT_COMPILER_H
#define AURASCRIPT_COMPILER_H

void aurascript_compiler_init(void);
void aurascript_init(void);
int aurascript_parse_cascade_layers(const char* layer_stream);
int aurascript_compile_master(void);

#endif

void aurascript_dump_full_source(void);
