#!/bin/bash
set -e

echo "[1/5] Cleaning stale object artifacts..."
rm -f *.o *.elf *.iso

echo "[2/5] Assembling Bootstrap & Compiling Subsystems..."
gcc -m32 -c boot.s -o boot.o
gcc -m32 -c idt_s.s -o idt_s.o
gcc -m32 -c kernel.c -o kernel.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c idt.c -o idt.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c keyboard.c -o keyboard.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c kernel_support.c -o kernel_support.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c command_registry.c -o command_registry.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c telemetry_handlers.c -o telemetry_handlers.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c audio_studio.c -o audio_studio.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c aura.c -o aura.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c aura_link.c -o aura_link.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c auracad.c -o auracad.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c aurascript.c -o aurascript.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c graphics.c -o graphics.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c net_stack.c -o net_stack.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c framebuffer.c -o framebuffer.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c studio_core.c -o studio_core.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c vfs.c -o vfs.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c ring_handshake.c -o ring_handshake.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c phasecore.c -o phasecore.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c phasecore_interpreter.c -o phasecore_interpreter.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c studiolens.c -o studiolens.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c spatial_compositor.c -o spatial_compositor.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c triconjectural.c -o triconjectural.o -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c enterprise_telemetry.c -o enterprise_telemetry.o -ffreestanding -O2 -Wall -Wextra

echo "[3/5] Linking Master Image..."
ld -m elf_i386 -T linker.ld -o torus_kernel.elf boot.o idt_s.o kernel.o idt.o keyboard.o kernel_support.o command_registry.o telemetry_handlers.o audio_studio.o aura.o aura_link.o auracad.o aurascript.o graphics.o net_stack.o framebuffer.o studio_core.o vfs.o ring_handshake.o phasecore.o phasecore_interpreter.o studiolens.o spatial_compositor.o triconjectural.o enterprise_telemetry.o --gc-sections

echo "[4/5] Setting up Bootloader & ISO Directory Structure..."
mkdir -p iso/boot/grub
cp torus_kernel.elf iso/boot/torus_kernel.elf
cat << 'GRUBEOF' > iso/boot/grub/grub.cfg
menuentry "TorusOS" {
    multiboot /boot/torus_kernel.elf
}
GRUBEOF

echo "[5/5] Packaging ISO with GRUB..."
grub-mkrescue -o torus-aura-master.iso iso

echo ""
echo "[=========================================================]"
echo "        TorusOS Master ISO & Kernel Built Successfully!    "
echo "[=========================================================]"
