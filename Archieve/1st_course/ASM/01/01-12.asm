%include "io.inc"

section .bss
	n resd 1
	m resd 1
	k resd 1
	d resd 1
	x resd 1
	y resd 1
	time resd 1

section .data
	res dd 1

section .text
	global CMAIN

CMAIN:
	GET_DEC 4, n
	GET_DEC 4, m
	GET_DEC 4, k
	GET_DEC 4, d
	GET_DEC 4, x
	GET_DEC 4, y

	mov eax, dword[n] ; eax = n
	mul dword[m] ; eax = n * m
	mul dword[k] ; edx_eax = n * m * k
	div dword[d] ; eax = (n * m * k) / d; edx = (n * m * k) % d

;	mov ecx, 1 ; ecx = 1
;	mov ebx, 0 ; ebx = 0
;	cmp edx, 0 
;	cmovnz ebx, ecx ; if (edx == 0) ebx = ecx = 1
;	add eax, ebx ; if (n * m * k) % d != 0 then eax = eax + 1
	neg edx
	sar edx, 31
	sub eax, edx

	mov dword[n], eax ; n = box amount

	sub eax, 1 ; eax = eax - 1

	mov edx, 0
	mov ebx, 3
	div ebx ; eax = (n - 1) / 3;
	add eax, 1 ; + zero box
;	mov ebx, 0
;	cmp dword[n], 0
;	cmovz eax, ebx ; only if n == 0 res = 0
	mov ebx, dword[n]
	neg ebx
	shr ebx, 31
	mul ebx
	mov dword[res], eax

	mov eax, dword[x]
	mov ebx, 60
	mul ebx ; eax = x * 60
	add eax, dword[y] ; eax = x * 60 + y
	mov dword[time], eax ; time = current time

;	mov eax, dword[res]
;	mov ebx, 0
;	cmp dword[time], 360
;	cmovs eax, ebx
;	mov dword[res], eax
	mov edx, dword[time]
	sub edx, 360
	sar edx, 31
	add edx, 1
	mov eax, dword[res]
	mul edx
	mov dword[res], eax

	mov eax, dword[n]
	sub eax, dword[res]

	PRINT_DEC 4, eax
	NEWLINE

	xor eax, eax
	ret
