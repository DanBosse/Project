#!/bin/bash
set -e

echo "[build.sh] Invoking TorusOS build pipeline..."

echo "[1/5] Cleaning stale object artifacts..."
rm -f *.o torus_kernel.elf torus-aura-master.iso

echo "[2/5] Assembling Bootstrap..."
if [ -f "boot.s" ]; then
    as --32 boot.s -o boot.o
elif [ -f "boot.asm" ]; then
    nasm -f elf32 boot.asm -o boot.o
else
    echo "Error: No boot.s or boot.asm found!"
    exit 1
fi

echo "[3/5] Compiling Core Subsystems & Drivers..."
gcc -m32 -ffreestanding -O2 -I. -c firewall_subsystem.c -o firewall_subsystem.o
gcc -m32 -ffreestanding -O2 -I. -c np_handshake.c -o np_handshake.o
gcc -m32 -ffreestanding -O2 -I. -c ring_handshake.c -o ring_handshake.o
gcc -m32 -ffreestanding -O2 -I. -c cli.c -o cli.o
gcc -m32 -ffreestanding -O2 -I. -c net_stack.c -o net_stack.o
gcc -m32 -ffreestanding -O2 -I. -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -O2 -I. -c kernel_utils.c -o kernel_utils.o
gcc -m32 -ffreestanding -O2 -I. -c ports.c -o ports.o
gcc -m32 -ffreestanding -O2 -I. -c vga.c -o vga.o
gcc -m32 -ffreestanding -O2 -I. -c spatial_compositor.c -o spatial_compositor.o
gcc -m32 -ffreestanding -O2 -I. -c vfs.c -o vfs.o
gcc -m32 -ffreestanding -O2 -I. -c mm.c -o mm.o
gcc -m32 -ffreestanding -O2 -I. -c ipc.c -o ipc.o
gcc -m32 -ffreestanding -O2 -I. -c trust_core_pqc.c -o trust_core_pqc.o
gcc -m32 -ffreestanding -O2 -I. -c enterprise_telemetry.c -o enterprise_telemetry.o
gcc -m32 -ffreestanding -O2 -I. -c studiolens_dashboard.c -o studiolens_dashboard.o
gcc -m32 -ffreestanding -O2 -I. -c command_registry.c -o command_registry.o
gcc -m32 -ffreestanding -O2 -I. -c kernel_stubs.c -o kernel_stubs.o

echo "[4/5] Linking Master Image with -n (nmagic)..."
ld -m elf_i386 -n -T linker.ld -o torus_kernel.elf boot.o firewall_subsystem.o np_handshake.o ring_handshake.o cli.o net_stack.o kernel.o kernel_utils.o ports.o vga.o spatial_compositor.o vfs.o mm.o ipc.o trust_core_pqc.o enterprise_telemetry.o studiolens_dashboard.o command_registry.o kernel_stubs.o

echo "[5/5] Setting up Bootloader & Packaging ISO..."
mkdir -p iso/boot/grub
cp torus_kernel.elf iso/boot/torus_kernel.elf
cat << 'GRUBEOF' > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "TorusOS Aura Master" {
    multiboot2 /boot/torus_kernel.elf
    boot
}
GRUBEOF

grub-mkrescue -o torus-aura-master.iso iso 2>/dev/null || xorriso -as mkisofs -R -b boot/grub/i386-pc/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o torus-aura-master.iso iso

echo ""
echo "========================================================="
echo "        TorusOS Master ISO & Kernel Built Successfully!  "
echo "========================================================="
