#include "kernel.h"
#include <stdint.h>

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void serial_print(const char* str);

// Simple helper to print hex values to serial
static void print_hex(uint64_t val) {
    char buf[19];
    buf[0] = '0';
    buf[1] = 'x';
    buf[18] = '\0';
    for (int i = 15; i >= 0; i--) {
        uint8_t nibble = val & 0xF;
        buf[2 + i] = (nibble < 10) ? ('0' + nibble) : ('a' + nibble - 10);
        val >>= 4;
    }
    serial_print(buf);
}

void gpf_handler(struct interrupt_frame *frame, uint64_t error_code) {
    serial_print("\n=========================================================\n");
    serial_print("[KERNEL PANIC / TRAP] Caught General Protection Fault (GPF #13)!\n");
    serial_print("[SANDBOX PROOF] Ring 3 tried to execute a privileged instruction (CLI).\n");
    serial_print("---------------------------------------------------------\n");
    serial_print("  Error Code (Selector/Flags): ");
    print_hex(error_code);
    serial_print("\n  Faulting RIP (User IP)     : ");
    print_hex(frame->rip);
    serial_print("\n  Code Segment (CS)          : ");
    print_hex(frame->cs);
    serial_print("\n  RFLAGS                     : ");
    print_hex(frame->rflags);
    serial_print("\n  User RSP                   : ");
    print_hex(frame->rsp);
    serial_print("\n=========================================================\n");

    while (1) {
        __asm__ volatile("cli; hlt");
    }
}
