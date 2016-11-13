%include "io.inc"

section .bss
	n resd 1
	m resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	GET_DEC 4, m

.LOOP:
	mov eax, dword[n]
	cmp eax, 0
	jz .END_LOOP
	mov ebx, eax
	mov edx, 0
	mov eax, dword[m]
	div ebx
	mov dword[n], edx
	mov dword[m], ebx
	jmp .LOOP

.END_LOOP:
	mov eax, dword[m]
	PRINT_DEC 4, eax

	xor eax, eax
	ret
