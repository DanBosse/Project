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
