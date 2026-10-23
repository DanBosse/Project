import re

# 1. Sanitize and fix kernel.c
with open("kernel.c", "r", encoding="utf-8") as f:
    content = f.read()

# Remove any broken IDT blocks, headers, or stray fragments
content = re.sub(r"#include <stdint.h>\s*", "", content)
content = re.sub(r"struct idt_entry \{.*?\};\s*", "", content, flags=re.DOTALL)
content = re.sub(r"struct idt_ptr \{.*?\};\s*", "", content, flags=re.DOTALL)
content = re.sub(r"void idt_set_gate\(.*?\}\s*", "", content, flags=re.DOTALL)
content = re.sub(r"static struct idt_entry idt\[256\];\s*", "", content)
content = re.sub(r"static struct idt_ptr idtp;\s*", "", content)
content = re.sub(r"extern void isr13_wrapper\(void\);\s*", "", content)
content = re.sub(r"serial_print\(buf\);\s*", "", content)

idt_block = """#include <stdint.h>

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

content = idt_block + "\n" + content

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

# 2. Fix gpf.c to ensure kernel.h is included
with open("gpf.c", "r", encoding="utf-8") as f:
    gpf_c = f.read()

if '#include "kernel.h"' not in gpf_c:
    gpf_c = '#include "kernel.h"\n' + gpf_c
    with open("gpf.c", "w", encoding="utf-8") as f:
        f.write(gpf_c)

print("Repair script completed successfully.")
