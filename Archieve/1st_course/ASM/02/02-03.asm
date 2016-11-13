%include "io.inc"

section .text
	global CMAIN

CMAIN:
	GET_UDEC 4, eax

	mov ebx, 0
	mov ecx, 0
	.LOOP:
		cmp ecx, 32
		je .END_LOOP
		mov edx, eax
		and edx, 1
		add ebx, edx
		shr eax, 1
		add ecx, 1
		jmp .LOOP
	.END_LOOP:

	PRINT_DEC 4, ebx
	NEWLINE		

	xor eax, eax
	ret
