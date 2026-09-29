CC = gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -m32 -ffreestanding -Iinclude -Wall -Wextra
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# Define build output directories
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
ISO_DIR = $(BUILD_DIR)/iso

# Explicitly list your root source folders to search through
SRC_DIRS = boot kernel mm drivers

# Find all .c and .asm files inside the specified root folders recursively
C_SRCS := $(shell find $(SRC_DIRS) -name '*.c')
AS_SRCS := $(shell find $(SRC_DIRS) -name '*.asm')

# Map source files to unique object paths inside build/bin/
# e.g., arch/x86/idt.c -> build/bin/arch/x86/idt.c.o
C_OBJS := $(patsubst %, $(BIN_DIR)/%.o, $(C_SRCS))
AS_OBJS := $(patsubst %, $(BIN_DIR)/%.o, $(AS_SRCS))
OBJS := $(C_OBJS) $(AS_OBJS)

.PHONY: all clean

all: kernel.iso

# Pattern rule for compiling C files
$(BIN_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for assembling Assembly files
$(BIN_DIR)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Link the kernel binary
$(BIN_DIR)/kernel.bin: $(OBJS) linker.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Generate the bootable ISO
kernel.iso: $(BIN_DIR)/kernel.bin grub.cfg
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN_DIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso $(ISO_DIR)

clean:
	rm -rf $(BUILD_DIR) kernel.iso

