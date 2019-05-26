; This is the exception de-multiplexer code.
; All low-level exception handling routines do the following:
;  1. disable interrupts
;  2. push error code on the stack (if the exception did not already
;     do so! (Some exceptions automatically push the error code onto the
;     stack.)
;  3. push the number of the exception onto the stack.
;  4. call the common interrupt service routine function, which then
;     branches back out based on the exception number on the stack.
;     (We do this because we don't want to replicate again and again the code 
;      to save the processor state.)
;

; Here come the interrupt service routines for the 32 exceptions.
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31


extern _promptA
extern _promptB
extern _promptC


;  0: Divide By Zero Exception
_isr0:
    push byte 0
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
_isr1:
    push byte 0
    push byte 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
_isr2:
    push byte 0
    push byte 2
    jmp isr_common_stub

;  3: Int 3 Exception
_isr3:
    push byte 0
    push byte 3
    jmp isr_common_stub

;  4: INTO Exception
_isr4:
    push byte 0
    push byte 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
_isr5:
    push byte 0
    push byte 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
_isr6:
    push byte 0
    push byte 6
    jmp isr_common_stub

;  7: Coprocessor Not Available Exception
_isr7:
    push byte 0
    push byte 7
    jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
_isr8:
    push byte 8
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
_isr9:
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
_isr10:
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
_isr11:
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
_isr12:
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
_isr13:
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
_isr14:
    push byte 14
    jmp isr_common_stub

; 15: Reserved Exception
_isr15:
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Exception
_isr16:
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception
_isr17:
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception
_isr18:
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
_isr19:
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
_isr20:
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
_isr21:
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
_isr22:
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
_isr23:
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
_isr24:
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
_isr25:
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
_isr26:
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
_isr27:
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
_isr28:
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
_isr29:
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
_isr30:
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
_isr31:
    push byte 0
    push byte 31
    jmp isr_common_stub



; The common stub below will pun out into C. Let the 
; assembler know that the function is defined in 'exceptions.C'.
extern _lowlevel_dispatch_exception

; This is the common low-level stub for the exception handler.
; It saves the processor state, sets up for kernel mode
; segments, calls the C-level exception handler, 
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
   
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, _lowlevel_dispatch_exception
    call eax	; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8	; Ceans up the pushed error code and pushed ISR number
    iret	; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP1
 


; load the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void _idt_load();'
; In turn, the variable '_idtp' is defined in file 'idt.C'.
global _idt_load
extern _idtp
_idt_load:
	lidt [_idtp]
	ret
