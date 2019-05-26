CSCE 410/611: MP6 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is strongly encouraged to study the contents of this file.

BOCH Environment:
=================

FILE:                   DESCRIPTION:

BIOS-bochs-latest       BIOS file.
VGABIOS-lgpl-latest     BIOS file for the graphics system.
dev_kernel_grub.img     Image file of the boot floopy.
bochsrc.bxrc            Description file for the system. 
                        Type 'bochs -f bochsrc.bxrc' to
                        start emulation.
c.img                   File representation for disk "C"
d.img                   and "D". "C" is connected to the
                        MASTER port on ATA-0,
                        "D" is connected to the SLAVE port.

COMPILATION:
===========

FILE:                   DESCRIPTION:

makefile (**)           Makefile for Linux 64-bit environment.
                        Works with the provided linux image. 
                        Type "make" to create the kernel.
linker.ld               The linker script.

OS COMPONENTS:
=============

FILE:                   DESCRIPTION:

start.asm (*)           The bootloader starts code in this file, which in turn
                        jumps to the main entry in File "kernel.C".
kernel.C (**)           Main file, where the OS components are set up, and the
                        system gets going.

assert.H/C              Implements the "assert()" utility.
utils.H/C               Various utilities (e.g. memcpy, strlen, etc..)

console.H/C             Routines to print to the screen.

machine.H (*)           Definitions of some system constants and low-level
                        machine operations. 
                        (Primarily memory sizes, register set, and
                        enable/disable interrupts)
gdt.H/C                 Global Descriptor Table.
gdt_low.asm             Low-level GDT code, included in "start.asm".
idt.H/C                 Interrupt Descriptor Table.
idt_low.asm             Low-level IDT code, included in "start.asm".
irq.H/C                 mapping of IRQ's into the IDT.
irq_low.asm             Low-level IRQ stuff. (Primarily the interrupt service
                        routines and the routine stub that branches out to the
                        interrupt dispatcher in "interrupts.C". Included in
                        "start.asm".
			
exceptions.H/C (*)      The exception dispatcher.
interrupts.H/C          The interrupt dispatcher.

console.H/C             Routines to print to the screen.

simple_timer.H/C (*)    Routines to control the periodic interval
                        timer. This is an example of an interrupt 
                        handler.

simple_keyboard.H/C(*)  Routines to access the keyboard. Primarily as
                        way to wait until user presses key.

simple_disk.H/C(**)     Simple LBA28 disk driver. Uses busy waiting
                        from operation issue until disk is ready
                        for data transfer. Use this class as 
                        base class for BlockingDisk.

file.H/C(**)     Implementation shell for the class File.

file_system.H/C(**) Implementation shell for class FileSystem.
			
machine_low.H/asm       Various low-level x86 specific stuff.


frame_pool.H/C          Definition and implementation of a
                        vanilla physical frame memory manager.
                        DOES NOT SUPORT contiguous
                        allocation. Also DOES NOT SUPPORT release
                        of frames.
                        FEEL FREE TO REPLACE THIS MANAGER WITH YOUR
                        OWN IMPLEMENTATION!!

mem_pool.H/C            Definition and implementation of a vanilla
                        memory manager.
                        DOES NOT SUPPORT release of memory.
                        FEEL FREE TO REPLACE THIS ABOMINATION WITH YOUR
                        OWN IMPLEMENTATION!!
			 

UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.sh (**)	Simple script to copy the kernel onto
	      		the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			In rare cases the paths in the file may need to be 
			edited to make them reflect the student's environment.

