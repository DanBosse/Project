path = "build.sh"
with open(path, "r") as f:
    c = f.read()

target = "qemu-system-x86_64 -M q35 -m 2G -vga std -cdrom torus-aura-master.iso -boot d -serial stdio"
replacement = "qemu-system-x86_64 -M q35 -m 2G -vga std -cdrom torus-aura-master.iso -boot d -serial stdio -no-reboot"

if target in c and "-no-reboot" not in c:
    c = c.replace(target, replacement)
    with open(path, "w") as f:
        f.write(c)
    print("[Success] Added -no-reboot to build.sh QEMU invocation.")
else:
    print("[Info] -no-reboot is already present or target line differs.")
