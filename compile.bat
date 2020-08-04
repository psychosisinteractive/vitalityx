@echo off
nasm -f elf k.asm -o ofiles/k.o
i686-elf-gcc -ffreestanding -c kernel.c -o ofiles/kernel.o
i686-elf-ld -o ofiles/kernel.bin -Ttext 0x1000 ofiles/k.o ofiles/kernel.o --oformat binary
i686-elf-ld -o ofiles/kernel.elf -Ttext 0x1000 ofiles/k.o ofiles/kernel.o 
nasm -f bin kickstart.asm -o ofiles/kickstart.bin
wsl cat ofiles/kickstart.bin ofiles/kernel.bin > vitality.bin
qemu-system-i386 -serial stdio -s -fda vitality.bin -d int -no-shutdown -no-reboot
exit