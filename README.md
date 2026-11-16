 
```markdown
# TorusSM

> **Project Status: On Hold (Early Bare-Metal Prototype)**  
> *TorusSM is an experimental **bare-metal state machine**—not a traditional operating system—exploring a 7-layer deterministic state architecture, geometric topology, and Lagrangian mechanics. It compiles as a standalone 32-bit freestanding binary (`torusos.bin`) that boots directly on x86 hardware (via QEMU/Multiboot) without an underlying OS. Further bare-metal hardware integration is currently paused pending dedicated test equipment.*

---

## Overview: State Machine, Not an Operating System

**TorusSM** (Torus State-Machine) is a bare-metal execution environment built around synchronized state transitions rather than traditional operating system abstractions (such as POSIX process trees, preemptive thread schedulers, or monolithic user-space/kernel-space stacks).

* **Why Bare-Metal?** Running freestanding (`-ffreestanding -nostdlib`) directly on x86 hardware eliminates host OS jitter, allowing the state machine to own hardware initialization, memory layout, and transition timing from boot.
* **The Architecture:** Subsystems are organized into a **7-layer state hierarchy** and coordinated through a **256-node Tensor Manifold Registry** (`Layer 2`), modeling system execution as structured state transitions inspired by geometric topology and Lagrangian mechanics ($L = T - V$).
* **Naming Note:** While the project and architecture are named **TorusSM**, the compiled binary (`torusos.bin`), boot banner, and interactive CLI prompt (`torusOS>`) retain the internal `torusOS` identifier from early bootstrap development.

---

## Current Implementation vs. Long-Term Vision

### Implemented Today (QEMU Bare-Metal Prototype)
* **32-Bit Freestanding Boot & Runtime:** Custom x86 assembly bootstrap (`boot.asm`), custom linker script (`linker.ld`), and freestanding C99 modules compiled into a bootable ISO (`torus-aura-master.iso`).
* **7-Layer State Initialization:** Initializes the 256-node Tensor Manifold Registry (`Layer 2`) and registers core state nodes across Layers 0–7.
* **Interactive State-Machine Shell (`torusOS>`):** Supports state inspection commands (`layers`, `dump`, `look`), active bus listings (`services`), tensor group routing matrices (`tgroups`), and interactive module menus (`physics`, `studiolens`).
* **Simulated Pipeline & Studio Telemetry:** Functional command stubs demonstrating multi-step state sweeps (`pipeline_full`), Hooke's Law spring calculations, and telemetry hooks across planned studio modules.

### Planned Roadmap (Future Work)
* **Direct Hardware I/O & Networking:** Bare-metal PCI enumeration, NIC drivers, and live packet state routing for the Layer 5 IPv4/TCP/ICMP/HTTP stack.
* **Hardware-Enforced Isolation:** MMU paging, privilege-ring separation, and watchdog-supervised state resets for deterministic fault recovery.
* **AuraScript Runtime:** A domain-specific scripting language designed to express state transitions, memory structures, mathematical notation, and Layer 7 graphical workspaces directly inside the state machine.
* **Integrated Creative Workspaces:** Graphical implementations of **StudioLens** (spatial graphics), **PhysicsLab** (60Hz constraint & multi-body simulation), **Audio/Video Studios**, and **Office/ASCII design tools**.

---

## 7-Layer State-Machine Architecture

| Layer | Name | State-Machine Role |
| :--- | :--- | :--- |
| **Layer 0** | **Bootstrap** | Multiboot entry, CPU mode setup, and bare-metal handoff |
| **Layer 1** | **Hardware Abstraction** | Direct memory, I/O port, and timer interfaces |
| **Layer 2** | **Tensor Manifold Registry** | 256-node topology registry tracking subsystem states and metadata |
| **Layer 3** | **Phase Core & Shell** | Core state-transition loop and interactive `torusOS>` CLI |
| **Layer 4** | **Trust Core** | Planned state attestation and transition gatekeeper |
| **Layer 5** | **Service Mesh & Pipeline** | Inter-state message buses (`ipc-router`, `telemetry-bus`, `manifold-tensor-core`) and tensor group routing |
| **Layer 6** | **Studio State Modules** | Isolated state environments for Physics, Graphics, Audio, Video, and StudioLens |
| **Layer 7** | **Display & Viewport** | Framebuffer output, system monitor, and AuraScript workspace |

---

## Building & Running (QEMU)

### Build from Source
Requires `gcc` (with 32-bit multilib support), `nasm`, and `ld`:

```bash
dan@workStation-01:~/Project$ make
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c kernel_main.c -o kernel_main.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c tensor_groups.c -o tensor_groups.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c services.c -o services.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c containers.c -o containers.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c physics_studio.c -o physics_studio.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c media.c -o media.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c network.c -o network.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c display_server.c -o display_server.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c system_monitor.c -o system_monitor.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c graphics_gui.c -o graphics_gui.o
nasm -f elf32 boot.asm -o boot.o
ld -m elf_i386 -nostdlib -T linker.ld kernel.o kernel_main.o tensor_groups.o services.o containers.o physics_studio.o media.o network.o display_server.o system_monitor.o graphics_gui.o boot.o -o torusos.bin

```

### Boot in QEMU

```bash
qemu-system-i386 -cdrom torus-aura-master.iso -m 2G -vga std -serial stdio

```

---

## Boot Log & Shell Reference

### Boot Sequence

```text
SeaBIOS (version 1.15.0-1)
iPXE ([https://ipxe.org](https://ipxe.org)) 00:03.0 CA00 PCI2.10 PnP PMM+07F8B340+07ECB340 CA00

Booting from ROM..==================================================
  TorusOS 7-Layer Architecture Booting...
==================================================
[Layer 2] Tensor Manifold Registry Initialized (256 Nodes).
[Registry] Registered subsystem: MultibootBootstrap
[Registry] Registered subsystem: TrustCoreGatekeeper
[Layer 5] Initializing Service Mesh & Pipeline Orchestrator...
[Layer 5] Internet Stack Initialized: IPv4, TCP/IP, ICMP & HTTP Daemon active.
[Layer 5] Service Mesh & AuraScript Engine Online.
[Layer 6] Media, Visual & StudioLens Engines Initialized.
[Layer 6] Physics Studio Module Initialized.
[Layer 6] Studio Container Daemon Initialized.
[Layer 7] Display Server Initialized.
[Kernel] TorusOS initialization sequence complete.

```

### Command Reference

```text
==================================================
  TorusOS 7-Layer Architecture Command Reference
==================================================
[Layer 0] Bootstrap:
  layers         - Display 7-layer structural breakdown
[Layer 2] Registry (Metadata & Topology):
  dump           - Dump 256-node tensor manifold status
[Layer 3] Phase Core State Machine & Shell:
  help           - Display this architectural help menu
  look           - Inspect current system tensor manifold & status
[Layer 4] Trust Core (Security Gatekeeper & Attestation):
  trust-status   - Query Layer 4 Trust Core attestation telemetry
  verify <work>  - Execute cryptographic workload attestation check
[Layer 5] Service Mesh, Manifold Core & Pipeline:
  services       - List active Layer 5 middleware buses
  tgroups        - Query Layer 5 tensor group routing matrix
  manifold-sync  - Synchronize Manifold Tensor Core across L5
  manifold-status- Query Manifold Tensor Core telemetry
  pipeline_full  - Run AuraSpring -> Graphics -> Audio -> Video pipeline
  net-status     - Query Internet & TCP/IP network stack telemetry
  ping <host>    - Transmit ICMP Echo Request across network stack
  http <url>     - Execute HTTP GET request via client daemon
[Layer 6] Containerized Studios & Workloads:
  studios        - List active studio containers
  spawn <name>   - Request Layer 4 crypto gate & launch studio
  physics        - Launch Physics Studio interactive module menu
  graphics       - Trigger frame rendering in GraphicsStudio
  audio          - Stream PCM audio buffer in AudioStudio
  video          - Decode video stream packet in VideoStudio
  studiolens     - Engage optical telemetry lens across studios
  aura           - Query Aura cognitive engine telemetry
  search <qry>   - Execute live query via Triject Search Engine
[Layer 7] Display Server & GUI:
  gui-init       - Initialize TorusOS Layer 7 Display Server
  sysmon         - Launch TorusOS System Monitor window
  gview          - Launch TorusOS Graphical Viewport window
  auragui        - Execute AuraScript GUI runtime script
==================================================

```

### Service Mesh, Tensor Groups & Pipeline Telemetry

```text
torusOS> services
--------------------------------------------------
  TorusOS Layer 5 Service Mesh & Pipeline Service
--------------------------------------------------
  [Bus: ipc-router] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: telemetry-bus] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: storage-daemon] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: media-pipeline-bus] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: triconjectural-bus] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: manifold-tensor-core] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: pipeline-orchestrator-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: aura-cognitive-bus] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: aurascript-engine] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: physics-gravity-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: physics-rt-solver-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: physics-xt-tensor-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: physics-spring-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: physics-collision-service] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: studiolens-optical-bus] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: network-stack-driver] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: tcp-ip-protocol] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: dns-resolver] Status: ROUTING & ACTIVE (Layer 5)
  [Bus: http-client-daemon] Status: ROUTING & ACTIVE (Layer 5)
--------------------------------------------------

torusOS> tgroups
--------------------------------------------------
  Layer 5 Tensor Group Interoperability Matrix
--------------------------------------------------
  Group 1 (Mass Hardware)      -> Storage Daemon & Network Driver
  Group 2 (Dynamic Wave)       -> Pipeline Service, Manifold Core, Gravity, RT & XT
  Group 3 (Obs. Topology)      -> Triconjectural, DNS, StudioLens, Aura Bus & AuraScript
  Group 4 (Gov. Integrity)     -> Trust Core Gateways
  Group 5 (Field Interaction)  -> IPC, TCP/IP, Spring & Collision Services
--------------------------------------------------

torusOS> pipeline_full
==================================================
  LAYER 5 SERVICE: pipeline_full ORCHESTRATION
==================================================
  [Step 1/4] Executing AuraSpring Tensor Oscillation...
[PHYSICS SPRING SERVICE] Hooke's Law oscillation computed (F = -kx).
  [Step 2/4] Rendering Graphics Studio Frame Buffer...
[GRAPHICS STUDIO] Frame buffer rendered. Resolution: 800x600.
  [Step 3/4] Streaming Audio Studio PCM Buffer...
[AUDIO STUDIO] PCM Audio Buffer streaming (44.1kHz).
  [Step 4/4] Decoding Video Studio Stream Packet...
[VIDEO STUDIO] Decoding stream packet @ 60fps.
--------------------------------------------------
  pipeline_full Multi-Step Sweep Complete. Stable.
==================================================

torusOS> studiolens
[StudioLens] Optical telemetry lens engaged across studios. Active.

torusOS> physics
==================================================
  TorusSM Physics Studio Interactive Module
==================================================
  Commands:
    sim     - Run multi-body particle lab simulation
    rt      - Trigger real-time 60Hz constraint solver
    xt      - Run high-precision tensor calculation
    stat    - Query physics engine telemetry
    exit    - Return to main torusOS> shell
==================================================

```

```

### Why This Framing Works So Well:
* **Eliminates false comparisons:** Explicitly stating *"not a traditional operating system"* right in the title block stops readers from expecting POSIX, UNIX permissions, or Windows/Linux desktop features.
* **Explains `torusOS>` naturally:** The **Naming Note** turns what looked like an inconsistency into an intentional detail (an internal bootstrap identifier inside a bare-metal state machine).
* **Justifies bare-metal C/ASM:** Explaining that `-ffreestanding` is used to avoid host OS jitter ties your technical implementation choices (`boot.asm`, `linker.ld`, `gcc -m32`) directly to your state-machine thesis.

```
  
  
  
  











 
