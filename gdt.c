#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct tss_entry {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed));

// GDT with 11 entries (0-8 normal selectors, 9-10 for 16-byte TSS descriptor)
static uint64_t gdt[11];
static struct gdt_ptr gp;
static struct tss_entry kernel_tss;
static uint8_t kstack[8192];

extern void gdt_flush(uint64_t, uint16_t);

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    struct gdt_entry *item = (struct gdt_entry *)&gdt[num];
    item->base_low = (base & 0xFFFF);
    item->base_middle = (base >> 16) & 0xFF;
    item->base_high = (base >> 24) & 0xFF;
    item->limit_low = (limit & 0xFFFF);
    item->granularity = ((limit >> 16) & 0x0F);
    item->granularity |= (gran & 0xF0);
    item->access = access;
}

static void gdt_set_system_gate(int num, uint64_t base, uint32_t limit, uint8_t access) {
    struct gdt_entry *item1 = (struct gdt_entry *)&gdt[num];
    item1->limit_low = (limit & 0xFFFF);
    item1->base_low = (base & 0xFFFF);
    item1->base_middle = (base >> 16) & 0xFF;
    item1->access = access;
    item1->granularity = ((limit >> 16) & 0x0F);
    item1->base_high = (base >> 24) & 0xFF;

    uint32_t *item2 = (uint32_t *)&gdt[num + 1];
    *item2 = (uint32_t)(base >> 32);
    *(item2 + 1) = 0;
}

void init_gdt(void) {
    gp.limit = (sizeof(uint64_t) * 11) - 1;
    gp.base = (uint32_t)&gdt;

    // 0: Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    // 1: Ring 0 Code (DPL=0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);
    // 2: Ring 0 Data (DPL=0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // 3: Ring 1 Code (DPL=1)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xBA, 0xAF);
    // 4: Ring 1 Data (DPL=1)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xB2, 0xCF);
    // 5: Ring 2 Code (DPL=2)
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xDA, 0xAF);
    // 6: Ring 2 Data (DPL=2)
    gdt_set_gate(6, 0, 0xFFFFFFFF, 0xD2, 0xCF);
    // 7: Ring 3 Code (DPL=3)
    gdt_set_gate(7, 0, 0xFFFFFFFF, 0xFA, 0xAF);
    // 8: Ring 3 Data (DPL=3)
    gdt_set_gate(8, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // Setup TSS
    for (int i = 0; i < sizeof(struct tss_entry); i++) {
        ((uint8_t *)&kernel_tss)[i] = 0;
    }
    kernel_tss.rsp0 = (uint32_t)(kstack + sizeof(kstack));
    kernel_tss.iopb_offset = sizeof(struct tss_entry);

    // 9-10: TSS Descriptor (16 bytes total starting at index 9)
    gdt_set_system_gate(9, (uint32_t)&kernel_tss, sizeof(struct tss_entry) - 1, 0x89);

    // Flush GDT and load TR (Task Register with selector 0x48 -> index 9 * 8 = 0x48)
    gdt_flush((uint32_t)&gp, 0x48);
}
