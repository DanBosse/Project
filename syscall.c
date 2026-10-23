#include <stdint.h>

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

// External serial print from kernel
extern void serial_print(const char* str);

void syscall_handler(struct interrupt_frame *frame) {
    (void)frame;
    serial_print("[Syscall] Ring 3 successfully invoked kernel via int 0x80!\n");
}
