# TorusOS Root Makefile

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -O2 -Wall

TRUST_CORE_LIB = trust_core/target/i686-unknown-linux-gnu/release/libtrust_core.a

TORUS_OBJS = boot.o idt_s.o kernel.o vga.o serial.o mm.o gdt.o gpf.o \
             kernel_utils.o command_registry.o ascii_xml.o \
             audio_studio.o aura.o aura_link.o auracad.o aurascript.o \
             enterprise_telemetry.o neural_ipc.o framebuffer.o graphics.o keyboard.o \
             net_stack.o phasecore.o phasecore_interpreter.o physics_telemetry.o \
             ring_handshake.o spatial_compositor.o studio_core.o studiolens.o \
             studiolens_dashboard.o triconjectural.o vfs.o kernel/torus_subsystems.o \
             cli.o

.PHONY: all clean

all: vfs_image.bin torus_kernel.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

physics_telemetry.o: trust_core/physics_telemetry.c
	$(CC) $(CFLAGS) -Itrust_core -I. -c $< -o $@

$(TRUST_CORE_LIB):
	cd trust_core && cargo build --release --target i686-unknown-linux-gnu

kernel/torus_subsystems.o: $(TRUST_CORE_LIB)
	mkdir -p kernel
	cp $(TRUST_CORE_LIB) kernel/torus_subsystems.o

torus_kernel.elf: $(TORUS_OBJS) linker.ld
	@echo "Linking TorusOS 32-bit Kernel ELF..."
	ld -m elf_i386 -T linker.ld -o torus_kernel.elf $(TORUS_OBJS)

clean:
	rm -f torus_kernel.elf kernel/torus_subsystems.o ascii_xml.o physics_telemetry.o *.o
	cd trust_core && cargo clean


# --- AuraScript VFS Desktop Bundling ---
AS_SOURCES = desktop_main.as user_terminal.as trinode_search.as
vfs_image.bin: $(AS_SOURCES)
	@echo "Packaging AuraScript desktop workspace into VFS initrd..."
	mkdir -p build/vfs/system
	cp desktop_main.as user_terminal.as trinode_search.as build/vfs/system/
	tar -cvf vfs_image.bin -C build/vfs .
