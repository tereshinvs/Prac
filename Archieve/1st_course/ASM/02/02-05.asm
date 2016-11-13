%include "io.inc"

section .bss
	A resd 10000
	B resd 10000
	C resd 10000
	n resd 1
	m resd 1
	k resd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	GET_DEC 4, m
	GET_DEC 4, k

	mov ecx, 0
	.LOOP1:
		cmp ecx, dword[n]
		je .END_LOOP1

		mov ebx, 0
		.LOOP2:
			cmp ebx, dword[m]
			je .END_LOOP2

			mov edx, ecx
			imul edx, dword[m]
			add edx, ebx
			GET_DEC 4, [A + 4 * edx]

			add ebx, 1
			jmp .LOOP2
		.END_LOOP2:

		add ecx, 1
		jmp .LOOP1
	.END_LOOP1:

	mov ecx, 0
	.LOOP3:
		cmp ecx, dword[m]
		je .END_LOOP3

		mov ebx, 0
		.LOOP4:
			cmp ebx, dword[k]
			je .END_LOOP4

			mov edx, ecx
			imul edx, dword[k]
			add edx, ebx
			GET_DEC 4, [B + 4 * edx]

			add ebx, 1
			jmp .LOOP4
		.END_LOOP4:

		add ecx, 1
		jmp .LOOP3
	.END_LOOP3:

	mov ecx, 0
	.LOOP5:
		cmp ecx, dword[n]
		je .END_LOOP5

		mov ebx, 0
		.LOOP6:
			cmp ebx, dword[k]
			je .END_LOOP6

			mov edx, ecx
			imul edx, dword[k]
			add edx, ebx
			mov dword[C + 4 * edx], 0   ;  обнуление C[i, j]

			mov edi, 0
			.LOOP7:
				cmp edi, dword[m]
				je .END_LOOP7

				mov esi, ecx
				imul esi, dword[m]
				add esi, edi
				mov eax, dword[A + 4 * esi] ; загрузка из A

				mov esi, edi
				imul esi, dword[k]
				add esi, ebx
				imul dword[B + 4 * esi] ; из B

				mov edx, ecx
				imul edx, dword[k]
				add edx, ebx
				add dword[C + 4 * edx], eax

				add edi, 1
				jmp .LOOP7
			.END_LOOP7:

			add ebx, 1
			jmp .LOOP6
		.END_LOOP6:

		add ecx, 1
		jmp .LOOP5
	.END_LOOP5:

	mov ecx, 0
	.LOOP8:
		cmp ecx, dword[n]
		je .END_LOOP8

		mov ebx, 0
		.LOOP9:
			cmp ebx, dword[k]
			je .END_LOOP9

			mov edx, ecx
			imul edx, dword[k]
			add edx, ebx
			mov eax, dword[C + 4 * edx]
			PRINT_DEC 4, eax
			PRINT_CHAR ' '

			add ebx, 1
			jmp .LOOP9
		.END_LOOP9:

		NEWLINE

		add ecx, 1
		jmp .LOOP8
	.END_LOOP8:

	xor eax, eax
	ret