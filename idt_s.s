.code32
.global idt_flush
.global keyboard_handler_stub
.extern keyboard_handler_main
.extern outb

idt_flush:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

keyboard_handler_stub:
    pusha
    
    # Call C-level keyboard handler
    call keyboard_handler_main
    
    # Send End of Interrupt (EOI) to Master PIC
    movb $0x20, %al
    outb %al, $0x20

    popa
    iret

.global gdt_flush

.global gdt_flush

.global gdt_flush

.global gdt_flush
gdt_flush:
    movl 4(%esp), %eax
    lgdt (%eax)
    movw 8(%esp), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    
    popl %ecx
    pushl $0x08
    pushl %ecx
    lret
