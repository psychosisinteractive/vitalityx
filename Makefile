C_SOURCES = $(wildcard libc/vitality/*.c libc/*.c drivers/*.c kernel/*.c)
HEADERS = $(wildcard libc/vitality/*.h libc/*.h drivers/*.h kernel/*.h)
A_SOURCES = ${wildcard kernel/libasm/*.s}
OBJ = ${C_SOURCES:.c=.o}
AOBJ = ${A_SOURCES:.s=.o}

CC = i686-elf-gcc
WSL = C:\Windows\sysnative\wsl
SYS = C:\Windows\sysnative\command
NASM = nasm
GDB = i686-elf-GDB
# flags for the compiler
CFLAGS = -g

.DEFAULT_GOAL := run

operating.bin: bootloader/boot.bin kernel.bin
	${WSL} cat $^ > operating.bin

kernel.bin: kernel/kentry.o ${OBJ} ${A_SOURCES}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: kernel/kentry.o ${OBJ} ${A_SOURCES}
	i686-elf-ld -o $@ -Ttext 0x1000 $^

run: operating.bin
	qemu-system-i386 -no-reboot -no-shutdown -fda operating.bin

debug: operating.bin
	qemu-system-i386 -d int -no-reboot -no-shutdown -s -fda operating.bin

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: ${A_SOURCES}
	${NASM} $< -f elf -o $@

%.o: %.asm
	${NASM} $< -f elf -o $@

%.bin: %.asm
	${NASM} $< -f bin -o $@

clean:
	${WSL} rm -rf *.bin *.dis *.o operating.bin *.elf
	${WSL} rm -rf kernel/*.o bootloader/*.bin bootloader/*.o libc/*.o libc/vitality/*.o
