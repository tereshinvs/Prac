#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stab.h>
#include <stdint.h>
#include <limits.h>

//#define DEBUG

enum
{
    MAX_FUNCTION_VALUE = 100000,
    MAX_FUNC_NAME_LNG = 8096
};

struct Stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uintptr_t n_value;
};

struct Function
{
    char *f_name;
    int f_index;
    uintptr_t f_start;
    uintptr_t f_end;
    char *f_file;
};

Elf32_Shdr *
get_section_by_name(char *elf_dump, char *name)
{
    Elf32_Ehdr header = *(Elf32_Ehdr *)elf_dump;
    Elf32_Shdr *name_sec = (Elf32_Shdr *)(elf_dump + header.e_shoff + header.e_shstrndx * header.e_shentsize);
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
cmp(const void *_t1, const void *_t2)
{
    struct Function *t1 = (struct Function *)_t1;
    struct Function *t2 = (struct Function *)_t2;
    int cmp_name = strcmp(t1->f_name, t2->f_name);
    if (cmp_name)
        return cmp_name;
    return t1->f_start - t2->f_start;
}

char *
strip(char *name)
{
    char *res = (char *)calloc(strlen(name) + 1, sizeof(char));

    strcpy(res, name);
    char *colon = strchr(res, ':');
    if (colon)
        *colon = 0;

    return res;
}

int
main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");

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

    char *elf_dump = (char *)calloc(elf_size + 1, sizeof(char));
    fread(elf_dump, sizeof(char), elf_size, in);
    fclose(in);

    #ifdef DEBUG
        printf("Elf readed\n");
        fflush(stdout);
    #endif

    #ifdef DEBUG
        Elf32_Ehdr *header = (Elf32_Ehdr *)elf_dump;
    #endif
    Elf32_Shdr *stab = get_section_by_name(elf_dump, ".stab");
    Elf32_Shdr *stabstr = get_section_by_name(elf_dump, ".stabstr");

    if (!stab || !stabstr || stab->sh_size == 0) {
        printf("No debug info\n");

        free(elf_dump);
        return 0;
    }

    struct Stab *stab_buf = (struct Stab *)(elf_dump + stab->sh_offset);

    struct Function *res = (struct Function *)calloc(MAX_FUNCTION_VALUE, sizeof(struct Function));
    int fun_k = 0;
    char *cur_file = NULL;

    #ifdef DEBUG
        printf("SH_NUM = %d\n", header->e_shnum);
        fflush(stdout);
    #endif

    for (int i = 0; i < stab->sh_size / sizeof(struct Stab); i++) {
        #ifdef DEBUG
            printf("SEC_NUM = %d   TYPE = %u\n", i, stab_buf[i].n_type);
            fflush(stdout);
        #endif

        if (stab_buf[i].n_type == N_FUN) {
            res[fun_k].f_name = strip(elf_dump + stabstr->sh_offset + stab_buf[i].n_strx);
            res[fun_k].f_index = i;
            res[fun_k].f_start = stab_buf[i].n_value;
            res[fun_k].f_end = 0;
            res[fun_k].f_file = cur_file;

            #ifdef DEBUG
                printf("Function %s\n", res[fun_k].f_name);
                fflush(stdout);
            #endif

            if (fun_k && res[fun_k - 1].f_end == 0)
                res[fun_k - 1].f_end = res[fun_k].f_start;

            fun_k++;
        }

        if (stab_buf[i].n_type == N_SO || stab_buf[i].n_type == N_SOL) {
            if (stab_buf[i].n_strx) {
                cur_file = elf_dump + stabstr->sh_offset + stab_buf[i].n_strx;
                #ifdef DEBUG
                    printf("File %s\n", cur_file);
                    fflush(stdout);
                #endif
            } else {
                if (fun_k && res[fun_k - 1].f_end == 0)
                    res[fun_k - 1].f_end = stab_buf[i].n_value;
            }
        }
    }

    qsort(res, fun_k, sizeof(struct Function), cmp);
    for (int i = 0; i < fun_k; i++) {
        printf("%s %d 0x%08x 0x%08x %s\n", res[i].f_name, res[i].f_index,
            res[i].f_start, res[i].f_end, res[i].f_file);
        free(res[i].f_name);
    }

    free(elf_dump);
    free(res);
    return 0;
}
