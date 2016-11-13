%include "io.inc"

section .bss
	n resd 1
	k resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	GET_DEC 4, k

	mov esi, 0 ; result

	mov ecx, 1
	.LOOP:
		cmp ecx, 
		ja
	.END_LOOP:

	PRINT_DEC 4, esi

	xor eax, eax
	ret
