CC = gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -m32 -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# Automatically find all .c and .asm files and map them to unique object names
C_OBJS = $(patsubst %.c, bin/%.c.o, $(wildcard *.c))
AS_OBJS = $(patsubst %.asm, bin/%.asm.o, $(wildcard *.asm))
OBJS = $(C_OBJS) $(AS_OBJS)

all: kernel.iso

# Pattern rule for C files (e.g., kernel.c -> bin/kernel.c.o)
bin/%.c.o: %.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for Assembly files (e.g., boot.asm -> bin/boot.asm.o)
bin/%.asm.o: %.asm
	@mkdir -p bin
	$(AS) $(ASFLAGS) $< -o $@

bin/kernel.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

kernel.iso: bin/kernel.bin grub.cfg
	mkdir -p iso/boot/grub
	cp bin/kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso

clean:
	rm -rf bin iso kernel.iso
