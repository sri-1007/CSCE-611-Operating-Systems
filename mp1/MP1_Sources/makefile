GCC_OPTIONS = -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables

all: kernel.elf

clean:
	rm -f *.o *.bin

# ==== KERNEL ENTRY POINT ====

start.o: start.asm
	nasm -f aout -o start.o start.asm

# ==== UTILITIES ====

utils.o: utils.H utils.C
	gcc $(GCC_OPTIONS) -g -c -o utils.o utils.C

# ==== DEVICES ====

console.o: console.H console.C
	gcc $(GCC_OPTIONS) -g -c -o console.o console.C

# ==== KERNEL MAIN FILE ====

kernel.o: kernel.C
	gcc $(GCC_OPTIONS) -g -c -o kernel.o kernel.C


kernel.elf: start.o kernel.o console.o utils.o linker.ld
	ld -melf_i386 -T linker.ld -o kernel.elf start.o kernel.o console.o utils.o

