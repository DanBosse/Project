.set MULTIBOOT2_HEADER_MAGIC, 0xe85250d6
.set MULTIBOOT_ARCHITECTURE_I386, 0
.set MULTIBOOT_HEADER_LENGTH, (header_end - header_start)
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + MULTIBOOT_HEADER_LENGTH)

.section .multiboot
header_start:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long MULTIBOOT_HEADER_LENGTH
    .long MULTIBOOT_CHECKSUM

    # Required Multiboot2 end tag
    .align 8
    .long 0
    .long 0
    .long 8
header_end:

.section .text
.global _start
.type _start, @function
_start:
    cli
    mov $stack_top, %esp
    
    # Call kernel main
    call kernel_main

_hang:
    hlt
    jmp _hang

.section .bss
.align 16
stack_bottom:
    .skip 16384 # 16 KB Stack
stack_top:
