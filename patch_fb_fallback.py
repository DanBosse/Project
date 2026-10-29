with open("spatial_compositor.c", "r") as f:
    code = f.read()

old_block = """void spatial_compositor_render(uint32_t flags) {
    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    uint32_t *fb = (uint32_t*)fb_info->address;"""

new_block = """void spatial_compositor_render(uint32_t flags) {
    uint32_t *fb = NULL;
    if (framebuffer_request.response && framebuffer_request.response->framebuffer_count > 0) {
        struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
        fb = (uint32_t*)fb_info->address;
    }
    if (!fb) {
        // Fallback standard LFB or safe memory area if bootloader FB is uninitialized
        fb = (uint32_t*)0xE0000000; 
    }"""

if old_block in code:
    code = code.replace(old_block, new_block)
    with open("spatial_compositor.c", "w") as f:
        f.write(code)
    print("Successfully added safe framebuffer fallback to spatial_compositor.c!")
else:
    print("Could not find exact block, checking alternative...")
