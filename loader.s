.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.global _start
.extern kernel_main

_start:
    cli
    mov $stack_top, %esp
    
    # Push multiboot info pointer (EBX)
    push %ebx
    call kernel_main

_halt:
    cli
    hlt
    jmp _halt

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:
