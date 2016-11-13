;
; Getting current clock number
;

section .text
	global timestamp

timestamp:
	RDTSC
	ret
