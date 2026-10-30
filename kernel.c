#include <stdint.h>
#include "kernel_utils.h"

// External CLI functions
extern void cli_execute(char *input);

static char input_buffer[256];
static int buffer_index = 0;

// External port read function (from ports.c)
extern uint8_t inb(uint16_t port);

void cli_print_prompt(void) {
    kernel_print("torus-aura> ");
}

void cli_handle_char(char c) {
    if (c == '\n' || c == '\r') {
        kernel_print("\n");
        input_buffer[buffer_index] = '\0';
        
        if (buffer_index > 0) {
            cli_execute(input_buffer);
            buffer_index = 0;
        }
        
        cli_print_prompt();
    } else if (c == '\b' || c == 127) { // Backspace
        if (buffer_index > 0) {
            buffer_index--;
            kernel_print("\b \b");
        }
    } else {
        if (buffer_index < (int)sizeof(input_buffer) - 1) {
            input_buffer[buffer_index++] = c;
            char s[2] = {c, '\0'};
            kernel_print(s);
        }
    }
}

// Check if serial data is ready on COM1 (port 0x3F8 + 5)
int serial_received(void) {
    return inb(0x3F8 + 5) & 1;
}

// Read character from COM1 (port 0x3F8)
char read_serial(void) {
    return inb(0x3F8);
}

void kernel_main(void) {
    kernel_print("\033[H\033[J");
    kernel_print("=========================================================\n");
    kernel_print("          TORUSOS AURA MASTER INITIALIZED (RING 0)       \n");
    kernel_print("=========================================================\n");
    
    cli_print_prompt();

    // Main kernel loop with serial polling
    while (1) {
        if (serial_received()) {
            char c = read_serial();
            cli_handle_char(c);
        }
        __asm__ volatile ("pause");
    }
}
