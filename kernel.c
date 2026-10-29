#include <stdint.h>
#include <stdbool.h>
#include "multiboot.h"

void vga_init(void);
void vga_print(const char* str);
void serial_init(void);
char serial_read(void);
void cli_print_prompt(void);
void cli_handle_char(char c);

void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch);
void spatial_compositor_render_frame(void);

void kernel_main(uint32_t magic, struct multiboot_info* mb_info) {
    (void)magic;
    vga_init();
    serial_init();
    vga_print("[TORUSOS] Initializing Spatial Compositor Architecture...\n");

    uint32_t *lfb = (uint32_t*)0xE0000000;
    uint32_t width = 1024, height = 768, pitch = 1024 * 4;

    if (mb_info && (mb_info->flags & (1 << 12))) {
        if (mb_info->framebuffer_addr != 0) {
            lfb = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
            width = mb_info->framebuffer_width;
            height = mb_info->framebuffer_height;
            pitch = mb_info->framebuffer_pitch;
            vga_print("[TORUSOS] Bootloader LFB Handover Successful.\n");
        }
    }

    // Initialize compositor with the active hardware LFB
    spatial_compositor_init(lfb, width, height, pitch);
    spatial_compositor_render_frame();

    vga_print("[TORUSOS] Spatial Compositor active. CLI integrated.\n");
    cli_print_prompt();
    
    while (1) {
        char c = serial_read();
        cli_handle_char(c);
        // Refresh the graphical frame on every key event / command loop iteration
        spatial_compositor_render_frame();
    }
}
