CPP = gcc
CPP_OPTIONS = -m32 -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fleading-underscore -fno-asynchronous-unwind-tables

all: kernel.bin

clean:
	rm -f *.o *.bin

start.o: start.asm 
	nasm -f aout -o start.o start.asm

utils.o: utils.C utils.H
	$(CPP) $(CPP_OPTIONS) -c -o utils.o utils.C

assert.o: assert.C assert.H
	$(CPP) $(CPP_OPTIONS) -c -o assert.o assert.C


# ==== VARIOUS LOW-LEVEL STUFF =====

machine.o: machine.C machine.H
	$(CPP) $(CPP_OPTIONS) -c -o machine.o machine.C

machine_low.o: machine_low.asm machine_low.H
	nasm -f aout -o machine_low.o machine_low.asm

# ==== DEVICES =====

console.o: console.C console.H
	$(CPP) $(CPP_OPTIONS) -c -o console.o console.C

# ==== MEMORY =====

cont_frame_pool.o: cont_frame_pool.C cont_frame_pool.H
	$(CPP) $(CPP_OPTIONS) -c -o cont_frame_pool.o cont_frame_pool.C

# ==== KERNEL MAIN FILE =====

kernel.o: kernel.C console.H 
	$(CPP) $(CPP_OPTIONS) -c -o kernel.o kernel.C


kernel.bin: start.o utils.o kernel.o assert.o console.o \
   cont_frame_pool.o machine.o machine_low.o  
	ld -melf_i386 -T linker.ld -o kernel.bin start.o utils.o \
   kernel.o assert.o console.o \
   cont_frame_pool.o  machine.o machine_low.o 
