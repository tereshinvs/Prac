%include "io.inc"

section .bss
	n resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	mov ebx, 1
	mov ecx, 2

.LOOP:
	mov eax, ecx
	mul ecx
	cmp eax, dword[n]
	jnbe .END_LOOP
	mov eax, dword[n]
	div ecx
	cmp edx, 0
	jne .NONZERO
	mov ebx, eax
	jmp .END_LOOP
.NONZERO:
	add ecx, 1
	jmp .LOOP

.END_LOOP:
	PRINT_DEC 4, ebx
	xor eax, eax
	ret