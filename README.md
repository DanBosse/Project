 TorusOS

TorusOS is an experimental operating system focused on fast boot, deterministic runtime behavior, spatial graphics, and a hardware-aware subsystem STATE-MACHINE architecture.

Unlike traditional operating systems, TorusOS is designed around synchronized subsystems, geometric modeling, and real-time state visualization. The project explores an architecture where independent modules operate through a unified lattice framework while maintaining fault isolation and autonomous recovery.

The long-term vision is a creative computing environment where applications, simulations, graphics, audio, AI, and development tools operate as integrated subsystems rather than disconnected software stacks.

Design Goals
Deterministic runtime execution
Rapid system startup and instant subsystem activation
Hardware-aware scheduling and routing
Spatial graphics and geometric visualization
Autonomous subsystem recovery and self-healing
Integrated AI-assisted workflows
Drag-and-drop application creation
Unified development environment for graphics, physics, audio, and simulation
Creative Environment

TorusOS is being designed as a complete creation platform rather than a traditional desktop environment.
With an extensive office, server, shared and internet capabilities.
Planned integrated environments include:

StudioLens (Spatial Graphics Studio)
PhysicsLab (Simulation & Modeling Studio)
Audio Studio
Video Studio
Graphics Studio
Office Studio
AI Development Environment
Geometry and ASCII-based design tools

The objective is to allow users to create software, simulations, visualizations, and interactive applications through integrated tooling with a command library intended to rival the usability of modern C++, Java, JavaScript, and Python ecosystems.

```bash
./build_iso.sh
qemu-system-x86_64 -cdrom torus-aura-master.iso -m 2G -vga std -serial stdio
```
