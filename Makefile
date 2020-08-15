C_SOURCES = $(wildcard libc/vitality/*.c libc/*.c drivers/*.c kernel/*.c libc/ext/*.c)
CPP_SOURCES = $(wildcard libc/g++/*.cpp)
HEADERS = $(wildcard libc/vitality/*.h libc/*.h drivers/*.h kernel/*.h libc/ext/*.h)
CPP_HEADERS = $(wildcard libc/g++/*.h)
A_SOURCES = ${wildcard kernel/libasm/*.s}
OBJ = ${C_SOURCES:.c=.o}
POBJ = ${CPP_SOURCES:.cpp=.o}
AOBJ = ${A_SOURCES:.s=.o}

CC = i686-elf-gcc
LD = i686-elf-ld
PP = i686-elf-g++
WSL = C:\Windows\sysnative\wsl
AS = i686-elf-as
SYS = C:\Windows\sysnative\command
BOCHS = C:\Program Files\Bochs-2.6.11\bochsdbg
NASM = nasm
GDB = i686-elf-GDB
# flags for the compiler
CFLAGS = -g -DSILENT

.DEFAULT_GOAL := run

ORIGIN = 0x7e00

operating.bin: bootloader/boot.bin kernel.bin
	${WSL} cat $^ > operating.bin

kernel.bin: kernel/kentry.o LINKER.ld ${OBJ} ${AOBJ}
	${LD} -o $@ -Ttext ${ORIGIN} $^ --oformat binary

kernel.elf: kernel/kentry.o LINKER.ld ${OBJ} ${AOBJ}
	${LD} -o $@ -Ttext ${ORIGIN} $^

run: operating.bin
	qemu-system-i386 -no-reboot -no-shutdown -fda operating.bin

debug: operating.bin
	${BOCHS}

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.cpp ${CPP_HEADERS}
	${PP} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.s
	${AS} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${NASM} $< -f elf -o $@

%.bin: %.asm bootloader/gdt.asm
	${NASM} $< -f bin -o $@

clean:
	${WSL} rm -rf *.bin *.dis *.o operating.bin *.elf
	${WSL} rm -rf kernel/*.o bootloader/*.bin bootloader/*.o libc/*.o libc/vitality/*.o
