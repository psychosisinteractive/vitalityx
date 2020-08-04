C_SOURCES = $(wildcard *.c)
HEADERS = $(wildcard libc/*.h)
# This is designed to compile under WINDOWS and WSL. CC and GDB are using Windows Compiled toolkits in the %PATH%
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

# Change this if your cross-compiler is somewhere else
CC = i686-elf-gcc
GDB = i686-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@


kernel.bin: ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: vitality.bin
	qemu-system-i386 -fda vitality.bin

vitality.bin: ofiles/kickstart.bin ofiles/kernel.bin
	C:\Windows\Sysnative\wsl cat $^ > vitality.bin

debug: vitality.bin kernel.elf
	qemu-system-i386 -s -fda vitality.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file ofiles/kernel.elf"

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case



# Open the connection to qemu and load our kernel-object file with symbols


# Generic rules for wildcards
# To make an object, always compile from its .c

clean:
	C:\Windows\Sysnative\wsl rm -rf *.bin *.dis *.o *.elf
	C:\Windows\Sysnative\wsl rm -rf libc/*.o *.o