C_SOURCES = $(wildcard libc/vitality/fs_implementations/*.c libc/vitality/*.c libc/*.c drivers/*.c kernel/*.c libc/ext/*.c)
CPP_SOURCES = $(wildcard libc/g++/*.cpp)
HEADERS = $(wildcard libc/vitality/fs_implementations/*.h libc/vitality/*.h libc/*.h drivers/*.h kernel/*.h libc/ext/*.h)
CPP_HEADERS = $(wildcard libc/g++/*.h)
A_SOURCES = ${wildcard kernel/libasm/*.s}
A2_SOURCES = ${wildcard kernel/libasm/*.asm}
OBJ = ${C_SOURCES:.c=.o}
LOBJ = ${LIBC_SOURCES:*.c=.o}
POBJ = ${CPP_SOURCES:.cpp=.o}
AOBJ = ${A_SOURCES:.s=.o}
A2OBJ = ${A2_SOURCES:.asm=.o}

CC = i386-elf-gcc
LD = i386-elf-ld
PP = i386-elf-g++
AS = i386-elf-as
SYS = C:\Windows\sysnative\command
BOCHS = C:\Program Files\Bochs-2.6.11\bochsdbg
#set this to cat if you are actually on linux/wsl
CAT = type
NASM = nasm
GDB = i686-elf-GDB
# flags for the compiler
CFLAGS = -g

.DEFAULT_GOAL := run

ORIGIN = 0x7e00

operating.bin: bootloader/boot.bin kernel.bin
	${CAT} $^ > operating.bin

kernel.bin: kernel/kentry.o LINKER.ld ${OBJ} ${AOBJ} ${A2OBJ}
	${LD} -o $@ -Ttext ${ORIGIN} $^ --oformat binary

kernel.elf: kernel/kentry.o LINKER.ld ${OBJ} ${AOBJ} ${A2OBJ}
	${LD} -o $@ -Ttext ${ORIGIN} $^

run: operating.bin
	qemu-system-i386 -no-reboot -no-shutdown -fda operating.bin -hda sys.img -boot a

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
