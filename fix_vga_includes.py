with open("vga.c", "r") as f:
    content = f.read()

# Remove our previously injected helper if it's at the very top
bad_helper = """static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}"""
content = content.replace(bad_helper, "")

# Ensure <stdint.h> is included, and insert outb properly after includes
io_helper = """
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
"""

if "#include <stdint.h>" not in content:
    content = "#include <stdint.h>\n" + content

if "static inline void outb" not in content:
    # Find position after last include or header block
    import re
    last_include = max([m.end() for m in re.finditer(r'#include\s+<[^>]+>', content)], default=0)
    if last_include > 0:
        content = content[:last_include] + "\n" + io_helper + content[last_include:]
    else:
        content = io_helper + content

with open("vga.c", "w") as f:
    f.write(content)

print("Cleaned and fixed vga.c successfully!")
