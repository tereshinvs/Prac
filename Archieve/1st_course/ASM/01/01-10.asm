%include "io.inc"

section .bss
	a resd 1
	k resb 1

section .text
	global CMAIN

CMAIN:
	GET_UDEC 4, a
	GET_DEC 1, k

	mov eax, dword[a]
	mov cl, 32
	sub cl, byte[k]
	shl eax, cl
	shr eax, cl

	PRINT_UDEC 4, eax
	NEWLINE

	xor eax, eax
	ret
