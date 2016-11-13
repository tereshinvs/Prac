#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stab.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "debug.h"

enum {
	MAX_SECTION_NAME_SIZE = 2048,
	MAX_FUNCTION_NAME_SIZE = 2048,
	MAX_FILE_NAME_SIZE = 2048
};

struct Stab {
	uint32_t n_strx; //позицияначаластрокивсекции.strstab
	uint8_t n_type; //типотладочногосимвола
	uint8_t n_other; //прочаяинформация
	uint16_t n_desc; //описаниеотладочногосимвола
	uintptr_t n_value;//значениеотладочногосимвола
};

int read_smth(int fd, void *dst, int len) {
	for (int i = 0; i < len; ) {
		int temp = read(fd, (char *)dst + i, len - i);
		if (temp <= 0)
			return 0;
		i += temp;
	}
	return 1;
}

void get_nth_section_name(int fd, char *dst, int n) {
//
// Read elf header
//
	lseek(fd, 0, SEEK_SET);
	Elf32_Ehdr header;
	read_smth(fd, &header, sizeof(Elf32_Ehdr));

	if (n >= header.e_shnum)
		return;
//
// Read need section header
//
	lseek(fd, header.e_shoff + n * header.e_shentsize, SEEK_SET);
	Elf32_Shdr sec_head;
	read_smth(fd, &sec_head, sizeof(Elf32_Shdr));
//
// Read string section header
//
	lseek(fd, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
	Elf32_Shdr str_sec_head;
	read_smth(fd, &str_sec_head, sizeof(Elf32_Shdr));
//
// Read string section
//
	lseek(fd, str_sec_head.sh_offset, SEEK_SET);
	char *str_sec_buf = (char *)calloc(str_sec_head.sh_size, 1);
	read_smth(fd, str_sec_buf, str_sec_head.sh_size);

	strcpy(dst, str_sec_buf + sec_head.sh_name);
	free(str_sec_buf);
}

void get_section_by_name(int fd, char *name, Elf32_Shdr *dst) {
	lseek(fd, 0, SEEK_SET);
	Elf32_Ehdr header;
	read_smth(fd, &header, sizeof(Elf32_Ehdr));

	for (int i = 0; i < header.e_shnum; i++) {
		char *temp = (char *)calloc(MAX_SECTION_NAME_SIZE, sizeof(char));
		get_nth_section_name(fd, temp, i);

		if (!strcmp(temp, name)) {
			free(temp);
			lseek(fd, header.e_shoff + i * header.e_shentsize, SEEK_SET);
			read_smth(fd, dst, sizeof(Elf32_Shdr));
			break;
		}

		free(temp);
	}
}

int load_stabs(int fd, struct Stab **stab_buf) {
	Elf32_Shdr stab_sec;
	get_section_by_name(fd, ".stab", &stab_sec);
	lseek(fd, stab_sec.sh_offset, SEEK_SET);
	*stab_buf = (struct Stab *)calloc(stab_sec.sh_size, sizeof(char));
	read_smth(fd, *stab_buf, stab_sec.sh_size);

	return stab_sec.sh_size / sizeof(struct Stab);
}

void load_stabstr(int fd, char **stabstr) {
	Elf32_Shdr stabstr_sec;
	get_section_by_name(fd, ".stabstr", &stabstr_sec);
	lseek(fd, stabstr_sec.sh_offset, SEEK_SET);
	*stabstr = (char *)calloc(stabstr_sec.sh_size, sizeof(char));
	read_smth(fd, *stabstr, stabstr_sec.sh_size);
}

void get_file_by_eip(struct Stab *stabs, int stab_len, char *stabstr,
	unsigned eip, char *file_dst) {
 
	int last_i = -1;
	for (int i = 0; i < stab_len; i++) {
		if (stabs[i].n_type == N_SO && stabs[i].n_value > eip)
			break;

		if (stabs[i].n_type == N_SO)
			last_i = i;
	}

	if (last_i >= 0)
		strcpy(file_dst, stabstr + stabs[last_i].n_strx);
	else
		strcpy(file_dst, "<unknown>");
}

void get_func_by_eip(struct Stab *stabs, int stab_len, char *stabstr,
	unsigned eip, char *func_dst, unsigned *first_func_instr) {

	int last_first_instr = 0, last_i = -1;

	for (int i = 0; i < stab_len; i++) {
		if (stabs[i].n_type == N_FUN && stabs[i].n_value > eip)
			break;

		if (stabs[i].n_type == N_FUN) {
			last_first_instr = stabs[i].n_value;
			last_i = i;
		}
	}

	if (last_i >= 0) {
		*first_func_instr = last_first_instr;
		strcpy(func_dst, stabstr + stabs[last_i].n_strx);
	} else
		strcpy(func_dst, "<unknown>");
}

void get_line_by_eip(struct Stab *stabs, int stab_len, unsigned first_func_instr,
	unsigned eip, int *line_dst) {

	int i = 0, last_func_i = 0;
	while (i < stab_len) {
		if (stabs[i].n_type == N_FUN && stabs[i].n_value > eip)
			break;
		if (stabs[i].n_type == N_FUN)
			last_func_i = i;
		i++;
	}
	i = last_func_i;

	int last_line_i = i;
	while (i < stab_len) {
		if (stabs[i].n_type == N_SLINE/* && stabs[i].n_value > eip - first_func_instr*/)
			break;
//		if (stabs[i].n_type == N_SLINE)
	//		last_line_i = i;
		i++;
	}
//	i = last_line_i;

	*line_dst = stabs[i].n_desc;
}

int get_main_ebp(struct Stab *stabs, int stab_len, char *stabstr, unsigned ebp) {
	while (ebp) {
		unsigned eip = *((unsigned *)(ebp + 4));
		ebp = *(unsigned *)ebp;

		if (ebp == 0)
			break;

		unsigned func_first_instr = 0;
		char *func_name = (char *)calloc(MAX_FUNCTION_NAME_SIZE, sizeof(char));
		get_func_by_eip(stabs, stab_len, stabstr, eip, func_name, &func_first_instr);
		if (strchr(func_name, ':'))
			*(strchr(func_name, ':')) = 0;

		if (!strcmp(func_name, "main"))
			return ebp;
		free(func_name);
	}
	return -1;
}

void backtrace(const char *name) {
	int fd = open(name, O_RDONLY);

	struct Stab *stab_buf;
	int stab_len = load_stabs(fd, &stab_buf);

	char *stabstr;
	load_stabstr(fd, &stabstr);

// Do magic with stack:
	unsigned ebp = (unsigned)&name - 8, eip;
	int main_ebp = get_main_ebp(stab_buf, stab_len, stabstr, ebp);

	printf("Stack backtrace:\n");
	while (ebp <= main_ebp) {
// Registers
		eip = *((unsigned *)(ebp + 4));
		ebp = *(unsigned *)ebp;

		if (ebp == 0)
			break;
// File name
		char *file_name = (char *)calloc(MAX_FILE_NAME_SIZE, sizeof(char));
		get_file_by_eip(stab_buf, stab_len, stabstr, eip, file_name);
// Function name
		unsigned func_first_instr = 0;
		char *func_name = (char *)calloc(MAX_FUNCTION_NAME_SIZE, sizeof(char));
		get_func_by_eip(stab_buf, stab_len, stabstr, eip, func_name, &func_first_instr);
		if (strchr(func_name, ':'))
			*(strchr(func_name, ':')) = 0;
// Line
		int line_num = 0;

		get_line_by_eip(stab_buf, stab_len, func_first_instr, eip, &line_num);
//
// Maybe line_num must be line_num - 1
//
		printf("ebp 0x%08x eip 0x%08x | %s:%d: %s\n",
			main_ebp - ebp, eip, file_name, line_num, func_name);

		free(func_name);
		free(file_name);
	}

	close(fd);
	free(stab_buf);
	free(stabstr);
}
