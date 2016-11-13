%include "io.inc"

section .bss
	n resd 1
	k resd 1
	a resd 40
	lng resd 1

section .data
	two dd 2

section .text
	global CMAIN

CMAIN:
	lea ecx, [esp + 4]
	and esp, -16
	push dword[ecx - 4]
	push ebp
	mov ebp, esp
	push ecx
	sub esp, 20

	GET_UDEC 4, n
	GET_UDEC 4, k

	mov eax, dword[n]
	mov ecx, 0
	.IN_LOOP:
		cmp eax, 0
		je .END_IN_LOOP

		mov edx, 0
		div dword[k]
		mov dword[a + 4 * ecx], edx

		add ecx, 1
		jmp .IN_LOOP
	.END_IN_LOOP:

	mov dword[lng], ecx

	.LOOP:
		call IS_LUCKY
		cmp eax, 0
		je .END_LOOP

		call NEXT_TICKET
		jmp .LOOP
	.END_LOOP:

	mov esi, 0
	mov ebx, 1
	mov ecx, 0
	.COUNT_RES_LOOP:
		cmp ecx, 40
		je .END_COUNT_RES_LOOP

		mov eax, dword[a + 4 * ecx]
		mul ebx
		add esi, eax

		mov eax, ebx
		mul dword[k]
		mov ebx, eax

		add ecx, 1
		jmp .COUNT_RES_LOOP
	.END_COUNT_RES_LOOP:

	PRINT_UDEC 4, esi
	NEWLINE

	mov eax, 0
	add esp, 20
	pop ecx
	pop ebp
	lea esp, [ecx - 4]
	ret

NEXT_TICKET:
	push ebp
	mov ebp, esp

	mov eax, dword[a]
	add eax, 1
	mov ebx, 0
	cmp eax, dword[k]
	jl ._OK
		sub eax, dword[k]
		mov ebx, 1
	._OK:
	mov dword[a], eax

	cmp eax, 0
	je ._NOT_END
		mov dword[lng], ecx
		add dword[lng], 1
	._NOT_END:

	mov ecx, 1
	.NT_LOOP:
		cmp ecx, 40
		je .END_NT_LOOP

		mov eax, dword[a + 4 * ecx]
		add eax, ebx

		mov ebx, 0
		cmp eax, dword[k]
		jl .OK
			sub eax, dword[k]
			mov ebx, 1
		.OK:

		mov dword[a + 4 * ecx], eax

		cmp eax, 0
		je .NOT_END
			mov dword[lng], ecx
			add dword[lng], 1
		.NOT_END:

		add ecx, 1
		jmp .NT_LOOP
	.END_NT_LOOP:

	leave
	ret

IS_LUCKY:
	push ebp
	mov ebp, esp

	mov eax, dword[lng]
	add eax, 1
	mov edx, 0
	div dword[two]
	mov ecx, eax
	sub ecx, 1

	mov edx, 0
	.IL_LOOP_LEFT:
		cmp ecx, 0
		jl .END_IL_LOOP_LEFT

		add edx, dword[a + 4 * ecx]

		sub ecx, 1
		jmp .IL_LOOP_LEFT
	.END_IL_LOOP_LEFT:

	mov ecx, eax
	.IL_LOOP_RIGHT:
		cmp ecx, dword[lng]
		je .END_IL_LOOP_RIGHT

		sub edx, dword[a + 4 * ecx]

		add ecx, 1
		jmp .IL_LOOP_RIGHT
	.END_IL_LOOP_RIGHT:

	mov eax, edx

	leave
	ret
