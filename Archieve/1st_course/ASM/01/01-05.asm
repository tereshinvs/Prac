%include "io.inc"

section .bss
	a resd 1
	b resd 1
	c resd 1
	d resd 1
	e resd 1
	f resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, a
	GET_DEC 4, b
	GET_DEC 4, c
	GET_DEC 4, d
	GET_DEC 4, e
	GET_DEC 4, f

	mov edx, 0
	mov ecx, 0					; result

	mov eax, dword[a]
	mov edx, dword[e]
	add edx, dword[f]
	mul edx						; eax = a * (e + f)
	add ecx, eax

	mov eax, dword[b]
	mov edx, dword[d]
	add edx, dword[f]
	mul edx						; eax = a * (e + f)
	add ecx, eax

	mov eax, dword[c]
	mov edx, dword[d]
	add edx, dword[e]
	mul edx						; eax = a * (e + f)
	add ecx, eax

	PRINT_UDEC 4, ecx
	NEWLINE

	xor eax, eax
	ret