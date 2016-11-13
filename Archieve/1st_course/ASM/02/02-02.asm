%include "io.inc"

section .bss
	a resd 1000001
	n resd 1
	k resd 1
	k1 resd 1
	k2 resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n

	mov ecx, 1
	.LOOP1:
		cmp ecx, dword[n]
		ja .END_LOOP1

		GET_UDEC 4, [a + 4 * ecx]

		add ecx, 1
		jmp .LOOP1
	.END_LOOP1:

	mov eax, dword[n]
	mov eax, dword[a + 4 * eax]
	mov dword[a], eax

	GET_DEC 4, k
	mov eax, 32
	sub eax, dword[k]
	mov dword[k2], eax
	mov ecx, dword[k]
	mov eax, 1
	shl eax, cl
	mov dword[k1], eax

	mov edi, 1
	.LOOP2:
		cmp edi, dword[n]
		ja .END_LOOP2

		mov eax, dword[a + 4 * edi - 4]
		mov edx, 0
		div dword[k1]

		mov ecx, dword[k2]
		shl edx, cl

		mov ebx, dword[a + 4 * edi]
		mov ecx, dword[k]
		shr ebx, cl

		add edx, ebx

		PRINT_UDEC 4, edx
		PRINT_CHAR ' '

		add edi, 1
		jmp .LOOP2
	.END_LOOP2:
	xor eax, eax
	ret
