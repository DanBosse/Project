import subprocess
import re

# 1. Restore kernel.c via git if possible
try:
    subprocess.run(["git", "checkout", "kernel.c"], check=True, stdout=subprocess.DEVNULL)
    print("Restored kernel.c from git baseline.")
except Exception:
    pass

with open("kernel.c", "r", encoding="utf-8") as f:
    content = f.read()

# 2. Prepend IDT structures and helper right at the top
idt_code = """#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t flags) {
    idt[vector].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[vector].selector = selector;
    idt[vector].ist = 0;
    idt[vector].flags = flags;
    idt[vector].offset_middle = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[vector].offset_high = (uint32_t)(handler >> 32);
    idt[vector].reserved = 0;
}
"""

content = idt_code + "\n" + content

# 3. Hook up vector 13 in the boot sequence
target = 'serial_print("[BOOT] Initializing Interrupt Descriptor Table (IDT)...\\n");'
addition = """
    extern void isr13_wrapper(void);
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint64_t)&idt;
    idt_set_gate(13, (uint64_t)isr13_wrapper, 0x08, 0x8E);
    __asm__ volatile("lidt %0" : : "m"(idtp));"""

if target in content and "isr13_wrapper" not in content.split("kernel_main")[1]:
    content = content.replace(target, target + addition, 1)

with open("kernel.c", "w", encoding="utf-8") as f:
    f.write(content)

# 4. Ensure gpf.c includes kernel.h
with open("gpf.c", "r", encoding="utf-8") as f:
    gpf = f.read()

if '#include "kernel.h"' not in gpf:
    gpf = '#include "kernel.h"\n' + gpf
    with open("gpf.c", "w", encoding="utf-8") as f:
        f.write(gpf)

print("Setup completed successfully!")
