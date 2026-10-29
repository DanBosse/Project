with open("build.sh", "r") as f:
    content = f.read()

# Replace the compilation and linking steps in build.sh with the complete set of object files
old_compilation = """gcc -m32 -c spatial_compositor.c -o spatial_compositor.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c triconjectural.c -o triconjectural.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c enterprise_telemetry.c -o enterprise_telemetry.o -ffreestanding -O2 -Wall -Wextra

echo "[3/5] Linking Master Image..."
ld -m elf_i386 -T linker.ld -o torus_kernel.elf boot.o idt_s.o kernel.o idt.o keyboard.o kernel_support.o command_registry.o telemetry_handlers.o audio_studio.o aura.o aura_link.o auracad.o aurascript.o graphics.o net_stack.o framebuffer.o studio_core.o vfs.o ring_handshake.o phasecore.o phasecore_interpreter.o studiolens.o spatial_compositor.o triconjectural.o enterprise_telemetry.o --gc-sections"""

new_compilation = """gcc -m32 -c spatial_compositor.c -o spatial_compositor.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c triconjectural.c -o triconjectural.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c enterprise_telemetry.c -o enterprise_telemetry.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c vga.c -o vga.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c serial.c -o serial.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c cli.c -o cli.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c mm.c -o mm.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c gdt.c -o gdt.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c gpf.c -o gpf.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c kernel_utils.c -o kernel_utils.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c studiolens_dashboard.c -o studiolens_dashboard.o -ffreestanding -O2 -Wall -Wextra

echo "[3/5] Linking Master Image..."
ld -m elf_i386 -T linker.ld -o torus_kernel.elf boot.o idt_s.o kernel.o idt.o keyboard.o kernel_support.o command_registry.o telemetry_handlers.o audio_studio.o aura.o aura_link.o auracad.o aurascript.o graphics.o net_stack.o framebuffer.o studio_core.o vfs.o ring_handshake.o phasecore.o phasecore_interpreter.o studiolens.o spatial_compositor.o triconjectural.o enterprise_telemetry.o vga.o serial.o cli.o mm.o gdt.o gpf.o kernel_utils.o studiolens_dashboard.o --gc-sections"""

if old_compilation in content:
    content = content.replace(old_compilation, new_compilation)
    with open("build.sh", "w") as f:
        f.write(content)
    print("Updated build.sh successfully!")
else:
    print("Could not find exact block in build.sh. Inspecting content...")
