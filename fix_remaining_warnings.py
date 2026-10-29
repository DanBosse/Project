# 1. Fix unused parameter 'magic' in kernel.c
with open("kernel.c", "r") as f:
    kernel_code = f.read()

# Cast or mark magic as unused
kernel_code = kernel_code.replace(
    'void kernel_main(uint32_t magic, struct multiboot_info* mb_info) {',
    'void kernel_main(uint32_t magic, struct multiboot_info* mb_info) {\n    (void)magic;'
)

with open("kernel.c", "w") as f:
    f.write(kernel_code)

# 2. Fix signedness warning in gdt.c
with open("gdt.c", "r") as f:
    gdt_code = f.read()

gdt_code = gdt_code.replace(
    'for (int i = 0; i < sizeof(struct tss_entry); i++) {',
    'for (unsigned int i = 0; i < sizeof(struct tss_entry); i++) {'
)

with open("gdt.c", "w") as f:
    f.write(gdt_code)

print("Remaining compiler warnings patched successfully!")
