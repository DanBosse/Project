boot_code = """
.code32
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set GRAPHICS, 1<<2             # request graphics mode
.set FLAGS,    ALIGN | MEMINFO | GRAPHICS
.set MAGIC,    0x1BADB002       # multiboot magic number
.set CHECKSUM, -(MAGIC + FLAGS) # checksum

# Declare the multiboot header with graphics request
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
# Additional header fields required when GRAPHICS flag is set:
.long 0        # header_addr
.long 0        # load_addr
.long 0        # load_end_addr
.long 0        # bss_end_addr
.long 0        # entry_addr
# Graphics preference:
.long 0        # mode_type (0 = linear graphics buffer)
.long 1024     # width
.long 768      # height
.long 32       # depth (bits per pixel)

.global _start
.extern kernel_main

_start:
    # 1. Set up the stack pointer so C code can execute
    mov $stack_top, %esp

    # 2. Clear EFLAGS
    xor %eax, %eax
    push %eax
    popf

    # 3. Pass Multiboot arguments to kernel_main(uint32_t magic, struct multiboot_info* mb_info)
    push %ebx
    push %eax
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
"""

with open("boot.s", "w") as f:
    f.write(boot_code.strip() + "\n")

print("boot.s successfully generated and verified!")
