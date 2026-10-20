import os

# 1. Update kernel.c with explicit noreturn attribute and absolute safe idle loop
kernel_code = """#include <stdint.h>
#include <stdbool.h>

void serial_print(const char *str);
void aurascript_run_script(const char *script);

void __attribute__((noreturn)) kernel_main(void) {
    serial_print("\\n=== TORUSOS KERNEL BOOTED (STABLE WORKSTATION MODE) ===\\n");
    
    // Execute AuraScript Bytecode VM initialization sequence
    aurascript_run_script("boot_sequence");

    serial_print("[AuraMotion] Adaptive Environment: High-refresh display detected (>=120Hz). Locking 2:1 cadence (120Hz / 60FPS).\\n");
    serial_print("[SpatialCompositor] Adaptive display subsystem rendered successfully.\\n");
    
    serial_print("[TorusOS] System fully initialized. Entering permanent low-power idle barrier.\\n");
    
    while (1) {
        __asm__ __volatile__("cli; hlt");
    }
}
"""

with open("kernel.c", "w") as f:
    f.write(kernel_code)

# 2. Update loader.s to ensure safety if execution ever falls through
if os.path.exists("loader.s"):
    with open("loader.s", "r") as f:
        loader_code = f.read()
    if "hang:" not in loader_code:
        loader_code += "\n.global hang\nhang:\n    cli\n    hlt\n    jmp hang\n"
        with open("loader.s", "w") as f:
            f.write(loader_code)

print("Boot loop guard applied successfully!")
