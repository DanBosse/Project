#include <stdint.h>
#include <stddef.h>

#define JIT_BUFFER_SIZE 4096

typedef struct {
    uint8_t code_buffer[JIT_BUFFER_SIZE];
    size_t offset;
    int is_compiled;
} aurascript_jit_context_t;

static aurascript_jit_context_t global_jit_ctx;

void aurascript_jit_init(void) {
    global_jit_ctx.offset = 0;
    global_jit_ctx.is_compiled = 0;
    for (size_t i = 0; i < JIT_BUFFER_SIZE; i++) {
        global_jit_ctx.code_buffer[i] = 0x90; // NOP padding
    }
}

int aurascript_jit_compile(const uint8_t *bytecode, size_t length) {
    if (!bytecode || length == 0 || length > JIT_BUFFER_SIZE) {
        return -1;
    }

    // Copy bytecode into JIT execution buffer
    for (size_t i = 0; i < length; i++) {
        global_jit_ctx.code_buffer[i] = bytecode[i];
    }
    
    global_jit_ctx.offset = length;
    global_jit_ctx.is_compiled = 1;
    return 0;
}

int64_t aurascript_jit_execute(void) {
    if (!global_jit_ctx.is_compiled) {
        return -1;
    }

    // Cast code buffer to an executable function pointer and execute
    int64_t (*jit_func)(void) = (int64_t (*)(void))(void *)global_jit_ctx.code_buffer;
    return jit_func();
}
