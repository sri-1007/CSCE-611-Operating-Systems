; File: machine_low.asm
;
; Low level CPU handling functions.
;
; September 3, 2012

; ----------------------------------------------------------------------
; get_EFLAGS()
; 
; Returns value of the EFLAGS status register. 
;
; ----------------------------------------------------------------------
global _get_EFLAGS
; this function is exported.
_get_EFLAGS:
	pushfd			; push eflags
	pop	eax		; pop contents into eax
	ret