CC = gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mcmodel=kernel -Iinclude
LDFLAGS = -nostdlib -T linker.ld

SRCS = kernel/torus_subsystems.c kernel/main.c
OBJS = $(SRCS:.c=.o)
TARGET = torusos.kernel

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

run: $(TARGET)
	qemu-system-x86_64 -kernel $(TARGET) -nographic

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
