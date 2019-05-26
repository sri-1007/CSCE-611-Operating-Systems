; File: threads_low.asm
;
; This code is a modified version of David Hovemeyer's GeekOS code.

; Low level interrupt/thread handling code for GeekOS.
; Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
; Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>


; ----------------------------------------------------------------------
; threads_low_switch_to(Thread * _thread)
; 
; If the calling entity is a thread, we save its context. 
; Then we load the context of the new thread, and continue executing
; with the new thread.
;
; ----------------------------------------------------------------------

[BITS 32]

KERNEL_CS equ 1<<3	; kernel code segment is GDT entry 1
KERNEL_DS equ 2<<3	; kernel data segment is GDT entry 2


INTERRUPT_STATE_SIZE equ 68 ; size of exception frame on stack

; Save registers prior to calling a handler function.
; This must be kept up to date with:
;   - REGS (register context) struct in machine.h
;   - setup_thread_context() in threads.c
%macro save_registers 0
	pushad
	push	ds
	push	es
	push	fs
	push	gs
%endmacro

; Restore registers and clean up the stack after calling a handler function
; (i.e., just before we return from the interrupt via an iret instruction).
%macro restore_registers 0
	pop	gs
	pop	fs
	pop	es
	pop	ds
        popad
	add	esp, 8	; skip int num and error code
%endmacro

extern _current_thread  ; defined and initialized in threads.c


global _threads_low_switch_to
align 16
; this function is exported.
_threads_low_switch_to:


        ; Is this a full-blown context switch, or is this just the start-up
	; thread giving control to the first real thread? If the latter, we 
	; don't need to do anything with setting up and saving the current
        ; context. We simply proceed to loading the new context. 

	cmp	[_current_thread], dword 0
	je	.context_load_only

	; Modify the stack to allow a later return via an iret instruction.
	; We start with a stack that looks like this:
	;
	;            thread_ptr
	;    esp --> return addr
	;
	; We change it to look like this:
	;
	;            thread_ptr
	;            eflags
	;            cs
	;    esp --> return addr

	push	eax		; save eax
	mov	eax, [esp+4]	; get return address

	mov	[esp-4], eax	; move return addr down 8 bytes from orig loc
	add	esp, 8		; move stack ptr up
	pushfd			; put eflags where return address was
	mov	eax, [esp-4]	; restore saved value of eax
	push	dword KERNEL_CS	; push cs selector
	sub	esp, 4		; point stack ptr at return address

	; Push fake error code and interrupt number
	push	dword 0
	push	dword 0

	; Save general purpose registers.
	save_registers

	; Save stack pointer in the thread context struct (at offset 0).
	mov	eax, [_current_thread]
	mov	[eax+0], esp

	; Load the pointer to the new thread context into eax.
	; We skip over the Interrupt_State struct on the stack to
	; get the parameter.
	mov	eax, dword [esp+INTERRUPT_STATE_SIZE]

	; Make the new thread current, and switch to its stack.
	mov	[_current_thread], eax
	mov	esp, [eax+0]

	; Restore general purpose and segment registers, and clear interrupt
	; number and error code.
	restore_registers

	; We'll return to the place where the thread was
	; executing last.
	iret

.context_load_only:

	; We skipped the whole exception frame setup, and just need to 
        ; store the thread pointer into eax.
        mov	eax, [esp+4]

	; Make the new thread current, and switch to its stack.
	mov	[_current_thread], eax
	mov	esp, [eax+0]

	; Restore general purpose and segment registers, and clear interrupt
	; number and error code.
	restore_registers

	; We'll return to the place where the thread was
	; executing last.
	iret
