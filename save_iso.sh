#!/bin/bash
set -e

echo "[1/4] Cleaning stale build artifacts..."
rm -f *.o trust_core/*.o

echo "[2/4] Compiling TorusOS subsystems and assembly entry point..."
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

gcc -ffreestanding -nostdlib -fno-pie -no-pie -m64 -mcmodel=kernel -mno-red-zone -c boot.s -o loader.o

echo "[3/4] Linking 64-bit Master Kernel executable..."
OBJS=$(find . -maxdepth 1 -name "*.o" ! -name "plugin.o" ! -name "plugin_bin.o")
ld -m elf_x86_64 -T linker.ld $OBJS -o iso_root/boot/torus-aura-kernel.elf

echo "[4/4] Writing Limine v8 boot image to torusos-release.iso..."
if [ -d "limine" ]; then
    cp limine/limine-bios-cd.bin limine/limine-uefi-cd.bin limine/limine-bios.sys iso_root/boot/limine/ 2>/dev/null || true
fi
if [ -f "limine.conf" ]; then
    cp limine.conf iso_root/
    cp limine.conf iso_root/boot/limine/
fi

xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o torusos-release.iso 2>/dev/null

echo "[SUCCESS] Standalone ISO saved to torusos-release.iso!"
