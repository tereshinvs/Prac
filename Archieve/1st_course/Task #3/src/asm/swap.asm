;
; Swapping two 64-bit numbers
;

section .text
	extern swaps
	global swap

swap:
	push ebp
	mov ebp, esp
	
	add dword[swaps], 1
	
	mov eax, dword[ebp + 8]
	mov edx, dword[ebp + 12]
;
; Now eax and edx will contain the first 32-bit of numbers
;	
	mov eax, dword[eax]
	mov edx, dword[edx]
	
	mov ecx, dword[ebp + 8]
	mov dword[ecx], edx
	
	mov ecx, dword[ebp + 12]
	mov dword[ecx], eax
;
; Now eax and edx will contain the second 32-bit of numbers
;	
	mov eax, dword[ebp + 8]
	mov edx, dword[ebp + 12]

	mov eax, dword[eax + 4]
	mov edx, dword[edx + 4]
	
	mov ecx, dword[ebp + 8]
	mov dword[ecx + 4], edx
	
	mov ecx, dword[ebp + 12]
	mov dword[ecx + 4], eax

	leave
	ret
