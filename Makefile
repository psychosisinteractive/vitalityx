C_SOURCES = $(wildcard libc/vitality/*.c libc/*.c drivers/*.c kernel/*.c libc/ext/*.c)
CPP_SOURCES = $(wildcard libc/g++/*.cpp)
HEADERS = $(wildcard libc/vitality/*.h libc/*.h drivers/*.h kernel/*.h libc/ext/*.h)
CPP_HEADERS = $(wildcard libc/g++/*.h)
A_SOURCES = ${wildcard kernel/libasm/*.s}
OBJ = ${C_SOURCES:.c=.o}
POBJ = ${CPP_SOURCES:.cpp=.o}
AOBJ = ${A_SOURCES:.s=.o}

CC = i686-elf-gcc
PP = i686-elf-g++
WSL = C:\Windows\sysnative\wsl
SYS = C:\Windows\sysnative\command
BOCHS = C:\Program Files\Bochs-2.6.11\bochsdbg
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

bochsdbg: operating.bin
	${BOCHS}

debug: operating.bin
	qemu-system-i386 -d int -no-reboot -no-shutdown -s -fda operating.bin

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.cpp ${CPP_HEADERS}
	${PP} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	${NASM} $< -f elf -o $@

%.bin: %.asm bootloader/gdt.asm
	${NASM} $< -f bin -o $@

clean:
	${WSL} rm -rf *.bin *.dis *.o operating.bin *.elf
	${WSL} rm -rf kernel/*.o bootloader/*.bin bootloader/*.o libc/*.o libc/vitality/*.o
