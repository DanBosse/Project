import re

# 1. Ensure gpf.c includes kernel.h properly
with open("gpf.c", "r", encoding="utf-8") as f:
    gpf = f.read()
gpf = gpf.replace('#include "kernel.h"\n', '')
with open("gpf.c", "w", encoding="utf-8") as f:
    f.write('#include "kernel.h"\n' + gpf)

# 2. Clean kernel.c by keeping everything from void kernel_main onwards
with open("kernel.c", "r", encoding="utf-8") as f:
    content = f.read()

idx = content.find("void kernel_main")
main_code = content[idx:] if idx != -1 else content

# Clean any duplicate/stray IDT injections inside kernel_main if present
main_code = re.sub(r"extern void isr13_wrapper\(void\);.*?\n", "", main_code)
main_code = re.sub(r"idt_set_gate\(13,.*?\);\s*", "", main_code)
main_code = re.sub(r"idtp\..*?;\s*", "", main_code)
main_code = re.sub(r'__asm__ volatile\("lidt.*?\);\s*', "", main_code)

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

target = 'serial_print("[BOOT] Initializing Interrupt Descriptor Table (IDT)...\\n");'
addition = """
    extern void isr13_wrapper(void);
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint64_t)&idt;
    idt_set_gate(13, (uint64_t)isr13_wrapper, 0x08, 0x8E);
    __asm__ volatile("lidt %0" : : "m"(idtp));"""

if target in main_code:
    main_code = main_code.replace(target, target + addition, 1)

with open("kernel.c", "w", encoding="utf-8") as f:
    f.write(idt_code + "\n" + main_code)

print("kernel.c and gpf.c fully sanitized and rebuilt.")
