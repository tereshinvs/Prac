%include "io.inc"

section .bss
	a resd 500000
	n resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	mov ecx, dword[n]
	cmp ecx, 0
	jne .NZ
		PRINT_DEC 4, ecx
		NEWLINE
		PRINT_DEC 4, ecx
		NEWLINE
		xor eax, eax
		ret
.NZ:
	mov ecx, 0
	.READ_LOOP:
		cmp ecx, dword[n]
		je .END_READ_LOOP
		GET_DEC 4, [a + ecx * 4]
		add ecx, 1
		jmp .READ_LOOP
	.END_READ_LOOP:

	sub dword[n], 1
	mov ebx, 0
	mov ecx, 1
	.MINIMAL_K_LOOP:
		cmp ecx, dword[n]
		je .END_MINIMAL_K_LOOP
		mov eax, dword[a + ecx * 4]
		cmp eax, dword[a + ecx * 4 - 4]
		jge .NOT_LOCAL_MINIMUM
			cmp eax, dword[a + ecx * 4 + 4]
			jge .NOT_LOCAL_MINIMUM
				add ebx, 1
		.NOT_LOCAL_MINIMUM:
		add ecx, 1
		jmp .MINIMAL_K_LOOP
	.END_MINIMAL_K_LOOP:
	PRINT_DEC 4, ebx
	NEWLINE
	mov ecx, 1
	.MINIMAL_LOOP:
		cmp ecx, dword[n]
		je .END_MINIMAL_LOOP
		mov eax, dword[a + ecx * 4]
		cmp eax, dword[a + ecx * 4 - 4]
		jge ._NOT_LOCAL_MINIMUM
			cmp eax, dword[a + ecx * 4 + 4]
			jge ._NOT_LOCAL_MINIMUM
				PRINT_DEC 4, ecx
				PRINT_CHAR ' '
		._NOT_LOCAL_MINIMUM:
		add ecx, 1
		jmp .MINIMAL_LOOP
	.END_MINIMAL_LOOP:
	NEWLINE

	mov ebx, 0
	mov ecx, 1
	.MAXIMUM_K_LOOP:
		cmp ecx, dword[n]
		je .END_MAXIMUM_K_LOOP
		mov eax, dword[a + ecx * 4]
		cmp eax, dword[a + ecx * 4 - 4]
		jle .NOT_LOCAL_MAXIMUM
			cmp eax, dword[a + ecx * 4 + 4]
			jle .NOT_LOCAL_MAXIMUM
				add ebx, 1
		.NOT_LOCAL_MAXIMUM:
		add ecx, 1
		jmp .MAXIMUM_K_LOOP
	.END_MAXIMUM_K_LOOP:
	PRINT_DEC 4, ebx
	NEWLINE
	mov ecx, 1
	.MAXIMUM_LOOP:
		cmp ecx, dword[n]
		je .END_MAXIMUM_LOOP
		mov eax, dword[a + ecx * 4]
		cmp eax, dword[a + ecx * 4 - 4]
		jle ._NOT_LOCAL_MAXIMUM
			cmp eax, dword[a + ecx * 4 + 4]
			jle ._NOT_LOCAL_MAXIMUM
				PRINT_DEC 4, ecx
				PRINT_CHAR ' '
		._NOT_LOCAL_MAXIMUM:
		add ecx, 1
		jmp .MAXIMUM_LOOP
	.END_MAXIMUM_LOOP:
	NEWLINE

	xor eax, eax
	ret
