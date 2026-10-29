with open("vga.c", "r") as f:
    content = f.read()

target = "void vga_init(void) {"
cursor_code = "\n    outb(0x3D4, 0x0A);\n    outb(0x3D5, 0x20);"

if target in content and "0x3D4" not in content:
    content = content.replace(target, target + cursor_code)
    with open("vga.c", "w") as f:
        f.write(content)
    print("Successfully patched vga.c to hide the text cursor!")
else:
    print("Cursor patch already applied or vga_init signature differs.")
