.global gdt_flush
.text
gdt_flush:
    lgdt (%rdi)       # Load GDT pointer passed in RDI

    # Load Task Register with TSS selector passed in RSI
    movw %si, %ax
    ltr %ax

    movw $0x10, %ax   # Ring 0 Data Segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Far jump to reload code segment (selector 0x08)
    lea .reload_cs(%rip), %rax
    pushq $0x08
    pushq %rax
    lretq

.reload_cs:
    ret
