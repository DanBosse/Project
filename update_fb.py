with open("kernel.c", "r") as f:
    content = f.read()

# Correct Limine Framebuffer Request UUIDs
old_target = """static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = { 0xcc67fb1a7dab6dbd, 0xc8a0b88885c08edf, 0x9d5827dcd881dd83, 0x62e78338d4bffd7b },
    .revision = 0,
    .response = NULL
};"""

new_target = """__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = { 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b, 0x9d5827dcd881dd75, 0xa3148604f6fab11b },
    .revision = 0,
    .response = NULL
};"""

if old_target in content:
    content = content.replace(old_target, new_target)
else:
    # Fallback replace if block formatting differs slightly
    content = content.replace("framebuffer_request = {", "framebuffer_request = {\n    .id = { 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b, 0x9d5827dcd881dd75, 0xa3148604f6fab11b },")

with open("kernel.c", "w") as f:
    f.write(content)

print("kernel.c updated with official Limine framebuffer magic IDs.")
