.global ring3_jump_test
.text
ring3_jump_test:
    cli                      # Disable interrupts during transition

    mov $0x43, %ax           # Ring 3 Data Selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Build IRETQ stack frame: SS, RSP, RFLAGS, CS, RIP
    pushq $0x43              # Stack segment
    mov %rsp, %rax
    pushq %rax               # Current stack pointer

    pushfq                   # Push current flags
    pop %rax
    or $0x200, %rax          # Ensure interrupts are enabled in RFLAGS
    push %rax

    pushq $0x3B              # Code segment
    leaq user_space_entry(%rip), %rax
    pushq %rax               # Instruction pointer to jump to

    iretq                    # Drop to Ring 3!

user_space_entry:
    # 1. Test safe kernel communication via system call
    int $0x80

    # 2. Safe completion / halt loop instead of privileged CLI trigger
    mov $1, %rax             # Exit syscall or graceful completion marker
    int $0x80
    jmp .
