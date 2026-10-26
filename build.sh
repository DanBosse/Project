#!/bin/bash
set -e

echo "[1/5] Cleaning stale object artifacts..."
rm -f *.o trust_core/*.o

echo "[2/5]
gcc -c ring_handshake.c -o ring_handshake.o -ffreestanding -O2
 Compiling TorusOS subsystems, trust core, and boot.s in 64-bit mode..."
mkdir -p iso_root/boot/limine

for c in *.c; do
    if [ "$c" = "mem.c" ] || [ "$c" = "aurascript_promotion.c" ]; then
        continue
    fi
    gcc -ffreestanding -nostdlib -fno-pie -no-pie -m64 -mcmodel=kernel -mno-red-zone -O2 -c "$c"
done

if [ -d "trust_core" ]; then
    for c in trust_core/*.c; do
        if [ -f "$c" ]; then
            gcc -ffreestanding -nostdlib -fno-pie -no-pie -m64 -mcmodel=kernel -mno-red-zone -O2 -c "$c" -o "$(basename ${c%.c}).o"
        fi
    done
fi

as ring3_asm.s -o ring3_asm.o
as gpf_asm.s -o gpf_asm.o
as gdt_asm.s -o gdt_asm.o
gcc -ffreestanding -nostdlib -fno-pie -no-pie -m64 -mcmodel=kernel -mno-red-zone -c boot.s -o loader.o

echo "[3/5] Linking 64-bit Master Image..."
OBJS=$(find . -maxdepth 1 -name "*.o" ! -name "plugin.o" ! -name "plugin_bin.o")
ld -m elf_x86_64 -T linker.ld $OBJS -o iso_root/boot/torus-aura-kernel.elf

echo "cp desktop_main.as studiolens_dashboard.as iso_root/ 2>/dev/null || true
[4/5] Copying Limine boot files and packaging ISO..."
if [ -d "limine" ]; then
    cp limine/limine-bios-cd.bin limine/limine-uefi-cd.bin limine/limine-bios.sys iso_root/boot/limine/ 2>/dev/null || true
fi
if [ -f "limine.conf" ]; then
    cp limine.conf iso_root/
    cp limine.conf iso_root/boot/limine/
fi

xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o torus-aura-master.iso 2>/dev/null

echo "[5/5] Build complete. ISO packaged successfully."
# qemu-system-x86_64 -M q35 -m 2G -vga std -cdrom torus-aura-master.iso -boot d -serial stdio -no-reboot
