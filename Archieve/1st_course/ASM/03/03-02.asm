%include "io.inc"

section .bss
	cur resd 11
	alr resd 11
	n resd 1
	m resd 1
	k resd 1
	cur_k resd 1

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

	GET_DEC 4, n
	GET_DEC 4, k
	GET_DEC 4, m

	call BRUTE_FORCE

	mov eax, 0
	add esp, 20
	pop ecx
	pop ebp
	lea esp, [ecx - 4]
	ret

BRUTE_FORCE:
	push ebp
	mov ebp, esp

	mov eax, dword[cur_k]
	cmp eax, dword[k]
	jne .NOT_K
		sub dword[m], 1

		mov eax, dword[m]
		cmp eax, 0
		jne .NOT_OUTPUT
			mov ecx, 0
			.OUT_LOOP:
				cmp ecx, dword[k]
				je .END_OUT_LOOP

				mov eax, dword[cur + 4 * ecx]
				PRINT_DEC 4, eax
				PRINT_CHAR ' '

				add ecx, 1
				jmp .OUT_LOOP
			.END_OUT_LOOP:
		.NOT_OUTPUT:

		leave
		ret
	.NOT_K:

	mov ecx, 1
	.LOOP:
		cmp ecx, dword[n]
		jg .END_LOOP

		mov eax, dword[alr + 4 * ecx]
		cmp eax, 1
		je .ALREADY
			mov eax, dword[cur_k]
			mov dword[cur + 4 * eax], ecx
			mov dword[alr + 4 * ecx], 1

			add dword[cur_k], 1
			push ecx
			call BRUTE_FORCE
			pop ecx
			sub dword[cur_k], 1

			mov dword[alr + 4 * ecx], 0
		.ALREADY:

		add ecx, 1
		jmp .LOOP
	.END_LOOP:

	leave
	ret
