#include <stdint.h>
#include "kernel.h"
#include "ipc.h"

#define PORT 0x3F8

extern void serial_init(void);
extern void serial_write(char c);
extern void init_idt(void);
extern uint8_t inb(uint16_t port);
extern void shell_dispatch_command(const char *cmd_line);

// Bind serial_print to use serial_write so command_registry.c can print natively
void serial_print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_write(str[i]);
    }
}

void kernel_init(void) {
    serial_init();     
    init_idt();        
    ipc_init();
}

void kernel_main(void) {
    kernel_init();

    serial_print("\r\n[TorusOS] Sovereign Kernel Booted Successfully! Type 'help' for options:\r\n> ");

    #define CMD_BUFFER_SIZE 128
    char cmd_buffer[CMD_BUFFER_SIZE];
    int cmd_index = 0;
    for (int i = 0; i < CMD_BUFFER_SIZE; i++) cmd_buffer[i] = 0;

    while (1) {
        if (inb(PORT + 5) & 0x01) {
            char c = inb(PORT);

            // Echo character back
            if (c >= 32 && c < 127) {
                if (cmd_index < CMD_BUFFER_SIZE - 1) {
                    cmd_buffer[cmd_index++] = c;
                    serial_write(c);
                }
            } 
            // Handle Backspace
            else if (c == '\b' || c == 127) {
                if (cmd_index > 0) {
                    cmd_index--;
                    cmd_buffer[cmd_index] = '\0';
                    serial_print("\b \b");
                }
            } 
            // Handle Enter (CR or LF)
            else if (c == '\r' || c == '\n') {
                serial_print("\r\n");
                cmd_buffer[cmd_index] = '\0';

                if (cmd_buffer[0] != '\0') {
                    // Dispatch directly to your master registry in command_registry.c
                    shell_dispatch_command(cmd_buffer);
                }

                // Reset buffer and print prompt again
                cmd_index = 0;
                for (int i = 0; i < CMD_BUFFER_SIZE; i++) cmd_buffer[i] = 0;
                serial_print("> ");
            }
        }
    }
}
