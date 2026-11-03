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
Geometry and ASCII-based design tools
AI Development Environment

Here is the complete, command reference, active services, tensor routing groups, pipeline execution, StudioLens telemetry, and Physics Studio interactive menu including the SeaBIOS and iPXE headers:

orus-aura-master.iso -m 2G -vga std -serial stdio

```text
rm -f *.o torusos.bin
dan@workStation-01:~/Project$ make
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c kernel_main.c -o kernel_main.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c tensor_groups.c -o tensor_grouo
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c services.c -o services.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c containers.c -o containers.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c physics_studio.c -o physics_sto
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c media.c -o media.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c network.c -o network.o
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c display_server.c -o display_seo
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c system_monitor.c -o system_mono
gcc -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99 -fno-pie -fno-stack-protector -c graphics_gui.c -o graphics_guio
nasm -f elf32 boot.asm -o boot.o
ld -m elf_i386 -nostdlib -T linker.ld kernel.o kernel_main.o tensor_groups.o services.o containers.o physics_studio.n
dan@workStation-01:~/Project$

The below is using Qemu
That is why the settings are low.

SeaBIOS (version 1.15.0-1)


iPXE (https://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+07F8B340+07ECB340 CA00



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
  TorusOS Physics Studio Interactive Module
==================================================
  Commands:
    sim     - Run multi-body particle lab simulation
    rt      - Trigger real-time 60Hz constraint solver
    xt      - Run high-precision tensor calculation
    stat    - Query physics engine telemetry
    exit    - Return to main torusOS> shell
==================================================

```
The remaing must be written in Aurascript.
AuraScript is a hybrid systems and application language designed specifically for TorusOS. It combines C-style memory access, C++ and Java object-oriented structures, JavaScript asynchronous execution, and Python-inspired expressiveness within a security-aware execution environment built around Ring 3 sandboxing, service-mesh communication, studio containers, math symbols, and Layer 7 graphical workspaces.

