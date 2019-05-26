global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; 32: IRQ0
_irq0:
    push byte 0
    push byte 32
    jmp irq_common_stub

; 33: IRQ1
_irq1:
    push byte 0
    push byte 33
    jmp irq_common_stub

; 34: IRQ2
_irq2:
    push byte 0
    push byte 34
    jmp irq_common_stub

; 35: IRQ3
_irq3:
    push byte 0
    push byte 35
    jmp irq_common_stub

; 36: IRQ4
_irq4:
    push byte 0
    push byte 36
    jmp irq_common_stub

; 37: IRQ5
_irq5:
    push byte 0
    push byte 37
    jmp irq_common_stub

; 38: IRQ6
_irq6:
    push byte 0
    push byte 38
    jmp irq_common_stub

; 39: IRQ7
_irq7:
    push byte 0
    push byte 39
    jmp irq_common_stub

; 40: IRQ8
_irq8:
    push byte 0
    push byte 40
    jmp irq_common_stub

; 41: IRQ9
_irq9:
    push byte 0
    push byte 41
    jmp irq_common_stub

; 42: IRQ10
_irq10:
    push byte 0
    push byte 42
    jmp irq_common_stub

; 43: IRQ11
_irq11:
    push byte 0
    push byte 43
    jmp irq_common_stub

; 44: IRQ12
_irq12:
    push byte 0
    push byte 44
    jmp irq_common_stub

; 45: IRQ13
_irq13:
    push byte 0
    push byte 45
    jmp irq_common_stub

; 46: IRQ14
_irq14:
    push byte 0
    push byte 46
    jmp irq_common_stub

; 47: IRQ15
_irq15:
    push byte 0
    push byte 47
    jmp irq_common_stub

extern _lowlevel_dispatch_interrupt

irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov eax, esp

    push eax
    mov eax, _lowlevel_dispatch_interrupt
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
