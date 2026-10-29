with open("vga.c", "r") as f:
    content = f.read()

# Add inline outb definition if missing
io_helper = """
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
"""

if "outb(" not in content or "static inline void outb" not in content:
    # Prepend it right after includes
    content = io_helper + content
    with open("vga.c", "w") as f:
        f.write(content)
    print("Added outb helper to vga.c successfully!")
else:
    print("outb helper already present.")
