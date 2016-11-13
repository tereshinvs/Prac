%include "io.inc"

section .bss
	a resd 1
	b resd 1
	c resd 1
	v resd 1
	k1 resd 1
	k2 resd 1
	k3 resd 1
	r1 resd 1
	r2 resd 1
	r3 resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, a
	GET_DEC 4, b
	GET_DEC 4, c
	GET_DEC 4, v

	mov eax, 0
	mov edx, 0

	mov eax, dword[a]
	div dword[v]
	mov dword[k1], eax
	mov dword[r1], edx

	mov eax, dword[b]
	mul dword[k1]
	mov dword[k2], eax
	mov eax, dword[r1]
	mul dword[b]
	div dword[v]
	add eax, dword[k2]
	mov dword[k2], eax
	mov dword[r2], edx

	mov eax, dword[c]
	mul dword[k2]
	mov dword[k3], eax
	mov eax, dword[r2]
	mul dword[c]
	div dword[v]
	add eax, dword[k3]
	mov dword[k3], eax
	mov dword[r3], edx

	PRINT_UDEC 4, eax
	NEWLINE

	xor eax, eax
	ret
