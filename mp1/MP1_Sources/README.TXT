CSCE 410/611/613: MP1 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is strongly encouraged to study the contents of this file.

BOCH Environment:
=================

FILE: 			DESCRIPTION:

BIOS-bochs-latest	BIOS file.
VGABIOS-lgpl-latest	BIOS file for the graphics system.
dev_kernel_grub.img	Image file of the boot floopy.
bochsrc.bxrc		Description file for the system. 
				(windows) doube-click this file to start emulation.
				(linux) type 'bochs -f bochsrc.bxrc' to
				start emulation.

COMPILATION:
===========

FILE: 			DESCRIPTION:

makefile 			Makefile for Linux 64-bit
				environment. Works with the provided
				linux image.
linker.ld			the linker script.

OS COMPONENTS:
=============

FILE: 			DESCRIPTION:

start.asm (*)		The bootloader starts code in this file, which in turn
			    	jumps to the main entry in File "kernel.C".
kernel.C (**)		Main file, where the OS components are set up, and the
                             	system gets going.
utils.H/C			Various utilities (e.g. memcpy, strlen, etc..)
console.H/C		Routines to print to the screen.

UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.sh (**)	(linux) Simple script to copy the kernel onto
	      			the floppy image on linux.
                        	The script mounts the floppy image, copies the kernel
				image onto it, and then unmounts the floppy image again.
  				The paths in the file may need to be edited to make them
				reflect the student's environment.
