;
; Comparing two 64-bit numbers
;

section .text
	extern comparings
	global less

less:
	push ebp
	mov ebp, esp
	
	add dword[comparings], 1
;
; First 32-bit half comparing
;
	mov eax, dword[ebp + 12]
	mov edx, dword[ebp + 20]

	cmp eax, edx
	je .LESS_SECOND_PART
		jl .LESS_LESS_1
			mov eax, 0
			jmp .LESS_END
		.LESS_LESS_1:
		mov eax, 1
		jmp .LESS_END
	.LESS_SECOND_PART:
;
; Second 32-bit half comparing
;
	mov eax, dword[ebp + 8]
	mov edx, dword[ebp + 16]
	
	cmp eax, edx
	jb .LESS_LESS_2
		mov eax, 0
		jmp .LESS_END
	.LESS_LESS_2:
	mov eax, 1

	.LESS_END:
	leave
	ret


	global greater

greater:
	push ebp
	mov ebp, esp
	
	add dword[comparings], 1
;
; First 32-bit half comparing
;
	mov eax, dword[ebp + 12]
	mov edx, dword[ebp + 20]

	cmp eax, edx
	je .GREATER_SECOND_PART
		jg .GREATER_GREATER_1
			mov eax, 0
			jmp .GREATER_END
		.GREATER_GREATER_1:
		mov eax, 1
		jmp .GREATER_END
	.GREATER_SECOND_PART:
;
; Second 32-bit half comparing
;
	mov eax, dword[ebp + 8]
	mov edx, dword[ebp + 16]
	
	cmp eax, edx
	ja .GREATER_GREATER_2
		mov eax, 0
		jmp .GREATER_END
	.GREATER_GREATER_2:
	mov eax, 1

	.GREATER_END:
	leave
	ret
