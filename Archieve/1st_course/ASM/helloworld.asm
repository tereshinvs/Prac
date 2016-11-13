%include "io.inc"

section .rodata
	msg db "Hello world", 0

section .text
	global CMAIN
	CEXTERN printf

CMAIN:
	lea ecx, [esp + 4]
	and esp, -16
	push dword[ecx - 4]
	push ebp
	mov ebp, esp
	push ecx
	sub esp, 20

	mov dword[esp], msg
	call printf

	mov eax, 0
	add esp, 20
	pop ecx
	pop ebp
	lea esp, [ecx - 4]
	ret
