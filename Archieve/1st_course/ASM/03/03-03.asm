%include "io.inc"

section .data
	three dd 3

section .text
	global CMAIN

CMAIN:
	lea ecx, [esp + 4]
	and esp, -16
	push dword[esp - 4]
	push ebp
	mov ebp, esp
	push ecx
	sub esp, 20

	GET_UDEC 4, eax

	push eax
	call COUNT
	add esp, 4

	PRINT_DEC 4, eax

	mov eax, 0
	add esp, 20
	pop ecx
	pop ebp
	lea esp, [ecx - 4]
	ret

COUNT:
	push ebp
	mov ebp, esp

	mov eax, dword[ebp + 8]
	mov edx, 0
	div dword[three]

	mov ecx, 0

	cmp edx, 1
	jne .NOT_ONE
		add ecx, 1
	.NOT_ONE:

	cmp eax, 0
	je .EAX_IS_ZERO
		push ecx
		push eax
		call COUNT
		add esp, 4
		pop ecx
		add ecx, eax
	.EAX_IS_ZERO:


	mov eax, ecx

	leave
	ret