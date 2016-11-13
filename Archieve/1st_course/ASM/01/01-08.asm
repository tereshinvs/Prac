%include "io.inc"

section .bss
	x1 resb 1
	y1 resb 1
	x2 resb 1
	y2 resb 1
	res resb 1
	tmp resb 1

section .text
	global CMAIN

CMAIN:
	GET_CHAR ah
	sub ah, 64
	mov byte[x1], ah
	GET_DEC 1, y1

	GET_CHAR ah
	GET_CHAR ah
	sub ah, 64
	mov byte[x2], ah
	GET_DEC 1, y2

	mov ah, byte[x1]
	sub ah, byte[x2]
	movsx eax, ah
	mov ecx, eax
	sar eax, 31
	mul ecx
	add eax, eax
	add eax, ecx
	mov byte[res], al

	mov ah, byte[y1]
	sub ah, byte[y2]
	movsx eax, ah
	mov ecx, eax
	sar eax, 31
	mul ecx
	add eax, eax
	add eax, ecx
	mov byte[tmp], al

	add al, byte[res]
	PRINT_DEC 1, al
	NEWLINE

	xor eax, eax
	ret