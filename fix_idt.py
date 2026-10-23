import re

# 1. Patch gpf.c to include kernel.h so serial_print is declared
with open("gpf.c", "r", encoding="utf-8") as f:
    gpf_content = f.read()

if "kernel.h" not in gpf_content:
    gpf_content = '#include "kernel.h"\n' + gpf_content
    with open("gpf.c", "w", encoding="utf-8") as f:
        f.write(gpf_content)
    print("Patched gpf.c with kernel.h")

# 2. Clean and patch kernel.c
with open("kernel.c", "r", encoding="utf-8") as f:
    k_content = f.read()

# Remove previous messy IDT injections if any
k_content = re.sub(r"struct idt_entry \{.*?\};\s*", "", k_content, flags=re.DOTALL)
k_content = re.sub(r"struct idt_ptr \{.*?\};\s*", "", k_content, flags=re.DOTALL)
k_content = re.sub(r"void idt_set_gate\(.*?\}\s*", "", k_content, flags=re.DOTALL)
k_content = re.sub(r"static struct idt_entry idt\[256\];\s*", "", k_content)
k_content = re.sub(r"static struct idt_ptr idtp;\s*", "", k_content)
k_content = k_content.replace("#include <stdint.h>\n", "")

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

k_content = idt_code + "\n" + k_content

target = 'serial_print("[BOOT] Initializing Interrupt Descriptor Table (IDT)...\\n");'
addition = """
    extern void isr13_wrapper(void);
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint64_t)&idt;
    idt_set_gate(13, (uint64_t)isr13_wrapper, 0x08, 0x8E);
    __asm__ volatile("lidt %0" : : "m"(idtp));"""

if target in k_content and "isr13_wrapper" not in k_content.split("kernel_main")[1]:
    k_content = k_content.replace(target, target + addition, 1)

with open("kernel.c", "w", encoding="utf-8") as f:
    f.write(k_content)

print("Successfully updated kernel.c with IDT setup and vector 13 handler registration!")
