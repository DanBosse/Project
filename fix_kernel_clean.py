import os
import subprocess

kernel_content = """#include <stdint.h>
#include "kernel.h"
#include "ipc.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void kernel_init(void) {
    // Initialize core IPC and ring handshakes (Milestone 3)
    ipc_init();
}

void kernel_main(void) {
    kernel_init();
    
    while (1) {
        __asm__ volatile ("pause");
    }
}
"""

print("[TORUS] Writing pristine, single-definition kernel.c...")
with open("kernel.c", "w") as f:
    f.write(kernel_content)

print("[TORUS] Cleaning stale object artifacts...")
for root, dirs, files in os.walk("."):
    for file in files:
        if file.endswith((".o", ".elf", ".bin")):
            try:
                os.remove(os.path.join(root, file))
            except Exception:
                pass

print("[TORUS] Invoking build script...")
subprocess.run(["./build.sh"], check=True)
print("[TORUS] Build successful!")
