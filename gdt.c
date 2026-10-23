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

static struct gdt_entry gdt[9];
static struct gdt_ptr gp;

extern void gdt_flush(uint64_t);

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void init_gdt(void) {
    gp.limit = (sizeof(struct gdt_entry) * 9) - 1;
    gp.base = (uint64_t)&gdt;

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

    gdt_flush((uint64_t)&gp);
}
