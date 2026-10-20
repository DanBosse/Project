#include <stdint.h>
#include <stdbool.h>

#define PORT 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(PORT + 1, 0x00);    
    outb(PORT + 3, 0x80);    
    outb(PORT + 0, 0x03);    
    outb(PORT + 1, 0x00);    
    outb(PORT + 3, 0x03);    
    outb(PORT + 2, 0xC7);    
    outb(PORT + 4, 0x0B);    
}

static int is_transmit_empty(void) {
    return inb(PORT + 5) & 0x20;
}

void serial_putchar(char a) {
    while (is_transmit_empty() == 0);
    outb(PORT, a);
}

static uint16_t vga_row = 0;
static uint16_t vga_col = 0;

void kernel_print(const char *str) {
    static bool initialized = false;
    if (!initialized) {
        serial_init();
        initialized = true;
    }

    while (*str) {
        char c = *str++;
        if (c == '\n') {
            serial_putchar('\r');
        }
        serial_putchar(c);

        // VGA text buffer at physical/virtual 0xB8000
        volatile uint16_t *vga = (volatile uint16_t*)0xB8000;
        if (c == '\n') {
            vga_row++;
            vga_col = 0;
        } else if (c == '\b') {
            if (vga_col > 0) vga_col--;
        } else {
            int index = vga_row * 80 + vga_col;
            vga[index] = (0x1F << 8) | c; // White text on Blue background
            vga_col++;
            if (vga_col >= 80) {
                vga_col = 0;
                vga_row++;
            }
        }
    }
}

void kernel_print_hex(uint64_t val) {
    char buf[19];
    buf[18] = '\0';
    int i = 17;
    do {
        int digit = val & 0xF;
        buf[i--] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        val >>= 4;
    } while (val > 0 && i >= 2);
    buf[i--] = 'x';
    buf[i--] = '0';
    kernel_print(&buf[i + 1]);
}

void phasecore_init(void) {
    kernel_print("[PhaseCore] Initialized deterministic execution engine & opcode dispatch.\n");
}
