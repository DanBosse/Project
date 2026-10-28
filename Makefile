# TorusOS Root Makefile

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -O2 -Wall

TRUST_CORE_LIB = trust_core/target/release/libtrust_core.a

# All compiled kernel and subsystem object files
TORUS_OBJS = boot.o kernel.o command_registry.o ascii_xml.o \
             audio_studio.o aura.o aura_link.o auracad.o aurascript.o \
             enterprise_telemetry.o framebuffer.o graphics.o \
             idt.o idt_s.o kernel_support.o keyboard.o net_stack.o \
             phasecore.o phasecore_interpreter.o ring_handshake.o \
             spatial_compositor.o studio_core.o studiolens.o \
             triconjectural.o vfs.o kernel/torus_subsystems.o

.PHONY: all clean

all: torus_kernel.elf

# Generic pattern rule for compiling C source files into 32-bit freestanding objects
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build the Rust trust_core crate
$(TRUST_CORE_LIB):
	cd trust_core && cargo build --release

# Link the Rust library into your kernel subsystems object
kernel/torus_subsystems.o: $(TRUST_CORE_LIB)
	mkdir -p kernel
	cp $(TRUST_CORE_LIB) kernel/torus_subsystems.o

# Build the true TorusOS 32-bit ELF kernel
torus_kernel.elf: $(TORUS_OBJS) linker.ld
	@echo "Linking TorusOS 32-bit Kernel ELF..."
	ld -m elf_i386 -T linker.ld -o torus_kernel.elf $(TORUS_OBJS)

clean:
	rm -f torus_kernel.elf kernel/torus_subsystems.o ascii_xml.o
	cd trust_core && cargo clean
