.global _start
.extern kernel_main

.section .text
_start:
    /* Set up a stack pointer */
    mov $stack_top, %rsp

    /* Jump to C kernel main */
    call kernel_main

    /* Hang if kernel ever returns */
.hang:
    cli
    hlt
    jmp .hang

.section .bss
.align 16
stack_bottom:
    .skip 16384 /* 16 KB stack */
stack_top:
