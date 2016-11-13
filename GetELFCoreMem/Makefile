all: elfcoremem.c elf_common.c elf_common.h
	gcc -o elfcoremem elfcoremem.c elf_common.c -std=gnu99 -m32

clean:
	-rm elfcoremem