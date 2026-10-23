.global gdt_flush
.text
gdt_flush:
    lgdt (%rdi)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    pushq $0x08
    lea .reload_cs(%rip), %rax
    pushq %rax
    lretq

.reload_cs:
    ret
