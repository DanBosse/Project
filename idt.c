#include <stdint.h>

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void idt_flush(uint32_t);
extern void keyboard_handler_stub(void);
extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void init_pic(void) {
    // ICW1: Initialize PICs
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Remap offsets (Master to 0x20 / 32, Slave to 0x28 / 40)
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3: Setup cascading
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // OCW1: Unmask IRQ 0 (timer) and IRQ 1 (keyboard) -> mask = 1111 1100 (0xFC)
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void init_idt(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    // Register keyboard handler at interrupt vector 33 (32 + IRQ 1)
    idt_set_gate(33, (uint32_t)keyboard_handler_stub, 0x08, 0x8E);

    init_pic();

    idt_flush((uint32_t)&idtp);
}
