%include "io.inc"

section .bss
	n resd 1
	x resd 1
	y resd 1
	tgcd resd 1
	res_x resd 1
	res_y resd 1

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

	mov dword[res_x], 0
	mov dword[res_y], 1

	GET_DEC 4, n

	mov ecx, 0
	.LOOP:
		cmp ecx, dword[n]
		je .END_LOOP

		GET_DEC 4, x
		GET_DEC 4, y

		mov eax, dword[res_x]    ;
		mul dword[y]            ; numerator 1
		mov dword[res_x], eax    ;

		mov eax, dword[x]			;
		mul dword[res_y]         ; numerator 2
		add dword[res_x], eax    ;

		mov eax, dword[res_y]		;
		mul dword[y]				; denomerator
		mov dword[res_y], eax    ;

		push ecx
		push dword[res_x]
		push dword[res_y]
		call GCD
		add esp, 8
		pop ecx
		PRINT_DEC 4, eax
		NEWLINE

		mov dword[tgcd], eax
		mov eax, dword[res_x]
		div dword[tgcd]
		mov dword[res_x], eax

		mov eax, dword[res_y]
		div dword[tgcd]
		mov dword[res_y], eax

	mov eax, dword[res_x]
	PRINT_DEC 4, eax
	PRINT_CHAR ' '
	mov eax, dword[res_y]
	PRINT_DEC 4, eax
	NEWLINE

		add ecx, 1
		jmp .LOOP
	.END_LOOP:

	mov eax, dword[res_x]
	PRINT_DEC 4, eax
	PRINT_CHAR ' '
	mov eax, dword[res_y]
	PRINT_DEC 4, eax

	mov eax, 0
	add esp, 20
	pop ecx
	pop ebp
	lea esp, [ecx - 4]
	ret

GCD:
	push ebp
	mov ebp, esp

	mov eax, dword[ebp + 8]
	mov edx, dword[ebp + 12]

	PRINT_STRING "GCD:"
	PRINT_DEC 4, eax
	PRINT_CHAR ' '
	PRINT_DEC 4, edx
	NEWLINE

	cmp edx, 0
	je .GCD_IS_HERE
		div edx
		mov eax, dword[ebp + 12]
		push edx
		push eax
		call GCD
		add esp, 8
	.GCD_IS_HERE:

	leave
	ret
