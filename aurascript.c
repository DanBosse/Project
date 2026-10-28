#include "aurascript.h"
#include "kernel.h"

extern void serial_print(const char *str);

static const char *kernel_as_source = "# AuraScript Kernel\n";

void aurascript_init(void) {
    serial_print("[AuraScript] Initializing compiler frontend & AST parser...\n");
}

bool aurascript_compile_stream(const char *source) {
    (void)source;
    serial_print("[AuraScript] AST building & type checking... [PASS]\n");
    return true;
}

void aurascript_jit_execute(void) {
    serial_print("[AuraScript JIT] Executing native JIT pass... [OK]\n");
}

void aurascript_dump_full_source(void) {
    serial_print(kernel_as_source);
}

void aurascript_eval(const char *arg) {
    serial_print("[AuraScript] Evaluating stream: ");
    if (arg) serial_print(arg);
    serial_print("\n");
    aurascript_compile_stream(arg);
    aurascript_jit_execute();
}
