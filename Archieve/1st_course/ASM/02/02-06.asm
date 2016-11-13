%include "io.inc"

section .bss
	x1 resd 1
	y1 resd 1
	x2 resd 1
	y2 resd 1
	_x resd 4
	_y resd 4
	x resd 1
	y resd 1

section .data
	yes dd "YES", 0
	no dd "NO", 0

section .text
	global CMAIN

CMAIN:
	mov ecx, 0
	.LOOP:
		cmp ecx, 4
		je .END_LOOP
		GET_DEC 4, [_x + ecx * 4]
		GET_DEC 4, [_y + ecx * 4]
		add ecx, 1
		jmp .LOOP
	.END_LOOP:
	GET_DEC 4, x
	GET_DEC 4, y

	mov eax, dword[_x]
	mov dword[x1], eax
	mov dword[x2], eax
	mov eax, dword[_y]
	mov dword[y1], eax
	mov dword[y2], eax

	mov ecx, 1
	.LOOP2:
		cmp ecx, 4
		je .END_LOOP2
		mov eax, dword[_x + ecx * 4]
		cmp eax, dword[x1]
		jge .NOT_LEFT
			mov dword[x1], eax
		.NOT_LEFT:
		cmp eax, dword[x2]
		jle .NOT_RIGHT
			mov dword[x2], eax
		.NOT_RIGHT:

		mov eax, dword[_y + ecx * 4]
		cmp eax, dword[y1]
		jge .NOT_DOWN
			mov dword[y1], eax
		.NOT_DOWN:
		cmp eax, dword[y2]
		jle .NOT_UP
			mov dword[y2], eax
		.NOT_UP:
		add ecx, 1
		jmp .LOOP2
	.END_LOOP2:

	mov eax, dword[x]
	cmp eax, dword[x1]
	jle .OUTSIDE
	cmp eax, dword[x2]
	jge .OUTSIDE
	mov eax, dword[y]
	cmp eax, dword[y1]
	jle .OUTSIDE
	cmp eax, dword[y2]
	jge .OUTSIDE
	PRINT_STRING yes
	xor eax, eax
	ret

	.OUTSIDE:
	PRINT_STRING no

	xor eax, eax
	ret
