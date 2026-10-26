#!/bin/bash
set -e

echo "[TORUS] Compiling all project C source files..."
for cfile in *.c; do
    if [ -f "$cfile" ]; then
        ofile="${cfile%.c}.o"
        gcc -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mcmodel=kernel -fno-pie -fno-PIC -fno-stack-protector -Iinclude -c "$cfile" -o "$ofile" 2>/dev/null || true
    fi
done

echo "[TORUS] Assembling assembly source files..."
for sfile in *.s; do
    if [ -f "$sfile" ]; then
        ofile="${sfile%.s}.o"
        as --64 "$sfile" -o "$ofile" 2>/dev/null || true
    fi
done

echo "[TORUS] Running Limine ISO build script..."
if [ -f build_iso.sh ]; then
    ./build_iso.sh
elif [ -f save_iso.sh ]; then
    ./save_iso.sh
fi

echo "[TORUS] Launching QEMU with Limine bootloader and serial console..."
if [ -f torus-aura-master.iso ]; then
    qemu-system-x86_64 -cdrom torus-aura-master.iso -m 2G -serial stdio -display none
elif [ -f torusos.hdd ]; then
    qemu-system-x86_64 -drive file=torusos.hdd,format=raw -m 2G -serial stdio -display none
else
    ISO_FILE=$(ls *.iso 2>/dev/null | head -n 1)
    if [ -n "$ISO_FILE" ]; then
        qemu-system-x86_64 -cdrom "$ISO_FILE" -m 2G -serial stdio -display none
    else
        echo "[ERROR] No Limine bootable ISO or disk image found!"
        exit 1
    fi
fi
