%include "io.inc"

section .data
	suit db 83, 67, 68, 72
	val db 50, 51, 52, 53, 54, 55, 56, 57, 84, 74, 81, 75, 65

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, eax
	sub eax, 1

	mov edx, 0
	mov ecx, 13
	div ecx

	mov dl, byte[val + edx]
	PRINT_CHAR dl
	mov al, byte[suit + eax]
	PRINT_CHAR al
	NEWLINE

	xor eax, eax
	ret
