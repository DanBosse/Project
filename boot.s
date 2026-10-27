.code32
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare the multiboot header so QEMU / GRUB recognizes the kernel
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.global _start
.extern kernel_main

_start:
    # 1. Set up the stack pointer so C code can execute
    mov $stack_top, %esp

    # 2. Clear EFLAGS
    xor %eax, %eax
    push %eax
    popf

    # 3. Jump into your C kernel entry point
    call kernel_main

    # 4. Hang if kernel_main returns
    cli
1:  hlt
    jmp 1b

# Allocate a small stack in the BSS section
.section .bss
.align 16
stack_bottom:
.skip 16384   # 16 KB stack
stack_top:
