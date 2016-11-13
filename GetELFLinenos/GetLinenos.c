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

enum
{
    MAX_LINE_VALUE = 1000000,
    MAX_FUNC_NAME_LNG = 4096
};

typedef struct Stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uintptr_t n_value;
} Stab;

typedef struct Line
{
    uint32_t l_foff; // Function address
    uint32_t l_addr; // Absolute address
    char *l_file;
    char *l_func;
    int l_line;
    int l_size;
} Line;

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

int
Line_comparator(const void *_t1, const void *_t2)
{
    Line *t1 = (Line *)_t1;
    Line *t2 = (Line *)_t2;

    if (t1->l_addr < t2->l_addr)
        return -1;
    if (t1->l_addr > t2->l_addr)
        return 1;

    return 0;
}

Line *
find_line_by_addr(Line *lines, int len, uint32_t addr)
{
    int left = 0, right = len - 1, med;
    Line *res = NULL;
    while (left <= right) {
        med = (left + right) / 2;
        if (lines[med].l_addr <= addr) {
            res = lines + med;
            left = med + 1;
        } else
            right = med - 1;
    }

    if (res && res->l_addr + res->l_size > addr)
        return res;
    else
        return NULL;
}

char *
strip(char *t)
{
    static char res[MAX_FUNC_NAME_LNG];

    strcpy(res, t);
    char *colon = strchr(res, ':');
    if (colon)
        *colon = 0;

    return res;
}

int
main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");

    fseek(in, 0, SEEK_END);
    uint32_t elf_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    char *elf_buf = (char *)calloc(elf_size, sizeof(char));
    fread(elf_buf, sizeof(char), elf_size, in);
    fclose(in);

    Elf32_Shdr *stab = get_section_by_name(elf_buf, ".stab");
    Elf32_Shdr *stabstr = get_section_by_name(elf_buf, ".stabstr");

    if (!stab || !stabstr) {
        free(elf_buf);
        printf("No debug info\n");
        return 0;
    }

    Stab *stab_buf = (Stab *)(elf_buf + stab->sh_offset);
    char *stabstr_buf = elf_buf + stabstr->sh_offset;

    Line *lines = (Line *)calloc(MAX_LINE_VALUE, sizeof(Line));
    int line_k = 0;
    char *cur_file = NULL, *cur_func = NULL;
    uint32_t cur_func_offset = 0;

    for (int i = 0; i < stab->sh_size / sizeof(Stab); i++) {
        if (stab_buf[i].n_type == N_SO || stab_buf[i].n_type == N_SOL) {
            if (!stab_buf[i].n_strx && line_k && lines[line_k - 1].l_size == -1)
                lines[line_k - 1].l_size = stab_buf[i].n_value - lines[line_k - 1].l_addr;

            cur_file = stabstr_buf + stab_buf[i].n_strx;
        }

        if (stab_buf[i].n_type == N_FUN) {
            if (line_k && lines[line_k - 1].l_size == -1)
                lines[line_k - 1].l_size = stab_buf[i].n_value - lines[line_k - 1].l_addr;

            cur_func = stabstr_buf + stab_buf[i].n_strx;
            cur_func_offset = stab_buf[i].n_value;
        }

        if (stab_buf[i].n_type == N_SLINE) {
            if (line_k && lines[line_k - 1].l_size == -1)
                lines[line_k - 1].l_size = stab_buf[i].n_value +
                    cur_func_offset - lines[line_k - 1].l_addr;

            lines[line_k].l_foff = cur_func_offset;
            lines[line_k].l_addr = stab_buf[i].n_value + cur_func_offset;
            lines[line_k].l_file = cur_file;
            lines[line_k].l_func = cur_func;
            lines[line_k].l_line = stab_buf[i].n_desc;
            lines[line_k].l_size = -1;

            line_k++;
        }
    }

    qsort(lines, line_k, sizeof(Line), Line_comparator);

    unsigned addr = 0;
    while (scanf("%x", &addr) != EOF) {
        Line *res = find_line_by_addr(lines, line_k, addr);
        if (res)
            printf("0x%08x:%s:%s:%x:%d\n", addr, res->l_file,
                strip(res->l_func), addr - res->l_foff, res->l_line);
        else
            printf("0x%08x::::\n", addr);
    }

    free(lines);
    free(elf_buf);
    return 0;
}
