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

//#define DEBUG

struct Stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uintptr_t n_value;
};

Elf32_Shdr *
get_section_by_name(char *elf_dump, char *name)
{
    Elf32_Ehdr header = *(Elf32_Ehdr *)elf_dump;
    Elf32_Shdr *name_sec = (Elf32_Shdr *)(elf_dump + header.e_shoff
        + header.e_shstrndx * header.e_shentsize);
    char *sec_names = elf_dump + name_sec->sh_offset;

    #ifdef DEBUG
        printf("Search section %s\n", name);
        fflush(stdout);
    #endif

    for (int i = 0; i < header.e_shnum; i++) {

        #ifdef DEBUG
            printf("Section num = %d", i);
            fflush(stdout);
        #endif

        Elf32_Shdr *cur_sec = (Elf32_Shdr *)(elf_dump + header.e_shoff + i * header.e_shentsize);

        #ifdef DEBUG
            printf("Current section %s\n", sec_names + cur_sec->sh_name);
            fflush(stdout);
        #endif

        if (strcmp(sec_names + cur_sec->sh_name, name) == 0)
            return cur_sec;
    }

    return NULL;
}

char *
get_file_by_eip(char *elf_dump, Elf32_Ehdr *header,
    Elf32_Shdr *stab, Elf32_Shdr *stabstr, unsigned addr)
{
    struct Stab *stab_buf = (struct Stab *)(elf_dump + stab->sh_offset);
    
    char *last_file_name = "<unknown>";
    for (int i = 0; i < stab->sh_size / sizeof(struct Stab); i++) {
        if (stab_buf[i].n_type == N_SO) {
            if (stab_buf[i].n_value > addr)
                break;
            last_file_name = elf_dump + stabstr->sh_offset + stab_buf[i].n_strx;
        }
    }

    return last_file_name;
}

char *
get_func_by_eip(char *elf_dump, Elf32_Ehdr *header,
    Elf32_Shdr *stab, Elf32_Shdr *stabstr, unsigned addr)
{
    struct Stab *stab_buf = (struct Stab *)(elf_dump + stab->sh_offset);
    
    char *last_file_name = "<unknown>";
    for (int i = 0; i < stab->sh_size / sizeof(struct Stab); i++) {
        if (stab_buf[i].n_type == N_FUN) {
            if (stab_buf[i].n_value > addr)
                break;
            last_file_name = elf_dump + stabstr->sh_offset + stab_buf[i].n_strx;
        }
    }

    return last_file_name;
}

int
get_line_by_eip(char *elf_dump, Elf32_Ehdr *header,
    Elf32_Shdr *stab, Elf32_Shdr *stabstr, unsigned addr)
{
    struct Stab *stab_buf = (struct Stab *)(elf_dump + stab->sh_offset);
    
    int last_line_num = 0;
    unsigned last_func_addr = 0;
    for (int i = 0; i < stab->sh_size / sizeof(struct Stab); i++) {
        if (stab_buf[i].n_type == N_FUN)
            last_func_addr = stab_buf[i].n_value;
        if (stab_buf[i].n_type == N_SLINE) {
            if (last_func_addr + stab_buf[i].n_value > addr)
                break;
            last_line_num = stab_buf[i].n_desc;
        }
    }

    return last_line_num;
}

unsigned
get_main_ebp(unsigned cur_ebp)
{
    while (cur_ebp)
        if (*(unsigned *)cur_ebp)
            cur_ebp = *(unsigned *)cur_ebp;
        else
            break;
    return cur_ebp;
}

char *
strip(char *name)
{
    static char res[1024];

    strcpy(res, name);
    char *colon = strchr(res, ':');
    if (colon)
        *colon = 0;

    return res;
}

void
backtrace(const char *name)
{
    FILE *in = fopen(name, "r");

    #ifdef DEBUG
        printf("File opened\n");
        fflush(stdout);
    #endif

    fseek(in, 0, SEEK_END);
    unsigned elf_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    #ifdef DEBUG
        printf("Elf size = %u\n", elf_size);
        fflush(stdout);
    #endif

    char *elf_dump = (char *)calloc(elf_size, sizeof(char));
    fread(elf_dump, elf_size, sizeof(char), in);
    fclose(in);

    #ifdef DEBUG
        printf("Elf readed\n");
        fflush(stdout);
    #endif

    Elf32_Ehdr *header = (Elf32_Ehdr *)elf_dump;
    Elf32_Shdr *stab = get_section_by_name(elf_dump, ".stab");
    Elf32_Shdr *stabstr = get_section_by_name(elf_dump, ".stabstr");

    #ifdef DEBUG
        printf("Sections stab and stabstr got\n", elf_size);
        fflush(stdout);
    #endif

    unsigned ebp = (unsigned)&name - 8, eip;
    unsigned main_ebp = get_main_ebp(ebp);

    printf("Stack backtrace:\n");

    while (ebp <= main_ebp) {
        eip = *((unsigned *)ebp + 1);
        ebp = *(unsigned *)ebp;

        if (ebp == 0)
            break;

        printf("ebp 0x%08x eip 0x%08x | %s:%d: %s\n", main_ebp - ebp, eip,
            get_file_by_eip(elf_dump, header, stab, stabstr, eip),
            get_line_by_eip(elf_dump, header, stab, stabstr, eip - 1),
            strip(get_func_by_eip(elf_dump, header, stab, stabstr, eip)));
    }

    free(elf_dump);
}
