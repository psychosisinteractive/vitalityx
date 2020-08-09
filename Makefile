C_SOURCES = $(wildcard libc/vitality/*.c libc/*.c drivers/*.c kernel/*.c)
HEADERS = $(wildcard libc/vitality/*.h libc/*.h drivers/*.h kernel/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

# Change this if your cross-compiler is somewhere else
CC = i686-elf-gcc
WSL = C:\Windows\sysnative\wsl
SYS = C:\Windows\sysnative\command
GDB = i686-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

# First rule is run by default
operating.bin: bootloader/boot.bin kernel.bin
	@${WSL} cat $^ > operating.bin ||:

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: kernel/kentry.o ${OBJ}
	@echo linking kentry and ${OBJ}
	@i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary ||:

# Used for debugging purposes
kernel.elf: kernel/kentry.o ${OBJ}
	@echo linking kentry and ${OBJ} (DEBUG)
	@i686-elf-ld -o $@ -Ttext 0x1000 $^ ||:

run: operating.bin
	@echo starting
	@qemu-system-i386 -no-reboot -no-shutdown -fda operating.bin ||:

# Open the connection to qemu and load our kernel-object file with symbols
debug: operating.bin
	@echo starting (DEBUG)
	@qemu-system-i386 -d int -no-reboot -no-shutdown -s -fda operating.bin ||:

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	@echo compiling C $< to $@
	@${CC} ${CFLAGS} -ffreestanding -c $< -o $@ ||:

%.o: %.asm
	@echo compiling ASM $< to $@
	@nasm $< -f elf -o $@ ||:

%.bin: %.asm
	@echo compiling ASM $< to $@
	@nasm $< -f bin -o $@ ||:

clean:
	@${WSL} rm -rf *.bin *.dis *.o operating.bin *.elf ||:
	@${WSL} rm -rf kernel/*.o bootloader/*.bin bootloader/*.o libc/*.o libc/vitality/*.o ||:
