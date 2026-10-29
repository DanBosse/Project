# TorusOS

TorusOS is an experimental operating system prototype focused on fast boot, deterministic runtime behavior, spatial graphics, and a hardware-aware subsystem model.

## Current Status & Milestones

TorusOS is currently at Milestone M18 ("Sovereign Convergence").

### What is real today:
- Bare-metal x86 boot path
- Multiboot kernel entry
- GRUB-based ISO packaging
- C/assembly kernel source tree
- Direct framebuffer rendering prototype (Tier 6 Spatial Compositor)
- PhysicsLab real-time simulation experiments
- Architecture for subsystem separation and accelerator routing

### Build and Run

```bash
./build_iso.sh
qemu-system-x86_64 -cdrom torus-aura-master.iso -m 2G -vga std -serial stdio
```
