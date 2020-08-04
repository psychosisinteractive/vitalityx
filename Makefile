C_SOURCES = $(wildcard *.c)
HEADERS = $(wildcard libc/*.h)
DRIVERS = $(wildcard drivers/*.h)
ASSEMBLIES = $(wildcard *.asm)
# This is designed to compile under WINDOWS and WSL. CC and GDB are using Windows Compiled toolkits in the %PATH%
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o} k.o
AOB = ${ASSEMBLIES:.asm=.o} 

# Change this if your cross-compiler is somewhere else
CC = i686-elf-gcc
GDB = i686-elf-gdb
ASM = nasm
# -g: Use debugging symbols in gcc
CFLAGS = -g

all: run

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.c ${DRIVERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.c ${C_SOURCES}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

kickstart.bin: boot/kickstart.asm
	nasm $< -f bin -o $@

k.o: k.asm
	nasm $< -f elf -o $@

%.o: %.asm ${ASSEMBLIES}
	nasm $< -f elf -o $@

ckern.bin: ${OBJ} k.o ${AOB}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

vitalityx.bin: kickstart.bin ckern.bin
	C:\Windows\Sysnative\wsl cat $^ > vitalityx.bin
# Used for debugging purposes
kernel.elf: ${OBJ} k.o ${AOB}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ 

run: vitalityx.bin
	qemu-system-i386 -d int -serial stdio -fda vitalityx.bin

debug: vitalityx.bin kernel.elf
	qemu-system-i386 -serial stdio -s -fda vitalityx.bin  -d int -no-shutdown -no-reboot &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case



# Open the connection to qemu and load our kernel-object file with symbols


# Generic rules for wildcards
# To make an object, always compile from its .c

clean:
	C:\Windows\Sysnative\wsl rm -rf *.bin *.dis *.o *.elf
	C:\Windows\Sysnative\wsl rm -rf libc/*.o *.o drivers/*.o