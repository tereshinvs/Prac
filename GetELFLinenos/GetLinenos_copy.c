#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <elf.h>
#include <stab.h>
#include <sys/procfs.h>
#include <sys/types.h>

enum
{
    MAX_LINE_VALUE = 1000000,
    MAX_FUNC_VALUE = 1000000
};

typedef struct ElfFile
{
    int fd;               // файловый дескриптор доступа к файлу
    Elf32_Ehdr *hdr;      // ELF-заголовок файла
    Elf32_Shdr *sections; // массив секции, NULL - если секций нет
    char *section_names;  // содержимое секции имен секций
    Elf32_Phdr *segments; // массив заголовков программ, NULL - если нет
} ElfFile;

typedef struct Stab
{
    uint32_t n_strx;   // позиция начала строки в секции .strstab
    uint8_t n_type;    // тип отладочного символа
    uint8_t n_other;   // прочая информация
    uint16_t n_desc;   // описание отладочного символа
    uint32_t n_value; // значение отладочного символа
} Stab;


typedef struct Line
{
    int line;
    uint32_t addr;
    uint32_t func_addr;
    char *file_name;
    char *func_name;
} Line;

typedef struct Func
{
    uint32_t begin, end;
    char *name;
} Func;

int
line_cmp(const void *_a, const void *_b)
{
    Line *a = (Line *)_a;
    Line *b = (Line *)_b;
    if (a->addr < b->addr) {
        return -1;
    } else if (a->addr > b->addr) {
        return 1;
    } else {
        return 0;
    }
}

int
func_cmp(const void *_a, const void *_b)
{
    Func *a = (Func *)_a;
    Func *b = (Func *)_b;
    if (a->begin < b->begin) {
        return -1;
    } else if (a->begin > b->begin) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t
min(uint32_t t1, uint32_t t2)
{
    return t1 < t2 ? t1 : t2;
}

void *
elf_load_section(const ElfFile *pelf, const Elf32_Shdr *ps)
{
    void *res = malloc(ps->sh_size);
    if (!res)
        return NULL;
    lseek(pelf->fd, ps->sh_offset, SEEK_SET);
    if (read(pelf->fd, res, ps->sh_size) != ps->sh_size) {
        free(res);
        return NULL;
    }
    return res;
}

ElfFile *
elf_open(const char *path)
{
    ElfFile *res = (ElfFile *)calloc(1, sizeof(ElfFile));
    // Open file
    res->fd = open(path, O_RDONLY);
    if (!res->fd)
        return NULL;
    // Read main hdr
    res->hdr = (Elf32_Ehdr *)calloc(1, sizeof(Elf32_Ehdr));
    read(res->fd, res->hdr, sizeof(*res->hdr));
    // Read section hdrs
    if (!res->hdr->e_shnum)
        res->sections = NULL;
    else {
        res->sections = (Elf32_Shdr *)calloc(res->hdr->e_shnum, sizeof(Elf32_Shdr));
        lseek(res->fd, res->hdr->e_shoff, SEEK_SET);
        read(res->fd, res->sections, res->hdr->e_shnum * sizeof(Elf32_Shdr));
        // Read section names
        res->section_names = elf_load_section(res, res->sections + res->hdr->e_shstrndx);
    }
    // Read program hdrs
    if (!res->hdr->e_phnum)
        res->segments = NULL;
    else {
        res->segments = (Elf32_Phdr *)calloc(res->hdr->e_phnum, sizeof(Elf32_Phdr));
        lseek(res->fd, res->hdr->e_phoff, SEEK_SET);
        read(res->fd, res->segments, res->hdr->e_phnum * sizeof(Elf32_Phdr));
    }
    return res;
}

ElfFile *
elf_close(struct ElfFile *pelf)
{
    // Close file
    close(pelf->fd);
    // Free resources
    free(pelf->hdr);
    free(pelf->section_names);
    if (pelf->sections)
        free(pelf->sections);
    if (pelf->segments)
        free(pelf->segments);
    // And free structure itself
    free(pelf);
    return NULL;
}

const Elf32_Shdr *
elf_find_section(const ElfFile *pelf, const char *section_name)
{
    for (int i = 0; i < pelf->hdr->e_shnum; ++i) {
        if (!strcmp(section_name, pelf->section_names + pelf->sections[i].sh_name)) {
            return &pelf->sections[i];
        }
    }
    return NULL;
}

void *
elf_load_segment(const ElfFile *pelf, const Elf32_Phdr *ph)
{
    void *res = malloc(ph->p_filesz);
    if (!res)
        return NULL;
    lseek(pelf->fd, ph->p_offset, SEEK_SET);
    if (read(pelf->fd, res, ph->p_filesz) != ph->p_filesz) {
        free(res);
        return NULL;
    }
    return res;
}

Elf32_Nhdr *
elf_note_begin(const Elf32_Phdr *pnote, void *pdata)
{
    Elf32_Nhdr *res = (Elf32_Nhdr *)pdata;
    return res;
}

Elf32_Nhdr *
elf_note_end(const Elf32_Phdr *pnote, void *pdata)
{
    Elf32_Nhdr *res = (Elf32_Nhdr *)pdata + pnote->p_filesz;
    return res;
}

void
strip(char *s)
{
    for (; *s; ++s)
        if (*s == ':') {
            *s = '\0';
            break;
        }
}

int
check(Func *a, int n, uint32_t addr)
{
    int l = 0;
    int r = n;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (a[m].begin <= addr)
            l = m;
        else
            r = m;
    }
    return a[l].begin <= addr && addr < a[l].end;
}

int
main(int argc, char *argv[])
{
    ElfFile *elf_file = elf_open(argv[1]);

    const Elf32_Shdr *stab_h = elf_find_section(elf_file, ".stab");
    const Elf32_Shdr *stabstr_h = elf_find_section(elf_file, ".stabstr");

    if (!stab_h || !stabstr_h) {
        printf("No debug info\n");
        elf_close(elf_file);
        return 0;
    }

    Line *lines = (Line *)calloc(MAX_LINE_VALUE, sizeof(Line));
    Func *funcs = (Func *)calloc(MAX_FUNC_VALUE, sizeof(Func));
    int line_k = 0, func_k = 0;
 
    char *cur_file = NULL;
    char *cur_func = NULL;
    uint32_t cur_func_addr = 0;
    Stab *stab = elf_load_section(elf_file, stab_h);
    char *stabstr = elf_load_section(elf_file, stabstr_h);

    int i = 0;
    for (Stab *is = stab; i < stab_h->sh_size / sizeof(Stab); i++, is++)
        switch (is->n_type) {
            case N_SO:
                if (func_k)
                    if (funcs[func_k - 1].end > is->n_value)
                        funcs[func_k - 1].end = is->n_value;
                cur_file = stabstr + is->n_strx;
                break;
            case N_SOL:
                cur_file = stabstr + is->n_strx;
                break;
            case N_FUN: {
                cur_func = stabstr + is->n_strx;
                strip(cur_func);
                cur_func_addr = is->n_value;
                
                Func tmp;
                tmp.begin = cur_func_addr;
                tmp.end = ~0;
                tmp.name = cur_func;
                if (func_k)
                    funcs[func_k - 1].end = min(funcs[func_k - 1].end, cur_func_addr);
                funcs[func_k++] = tmp;
                break;
            }
            case N_SLINE: {
                Line tmp;
                tmp.line = is->n_desc;
                tmp.addr = cur_func_addr + is->n_value;
                tmp.func_name = cur_func;
                tmp.func_addr = cur_func_addr;
                tmp.file_name = cur_file;
                for (int j = i + 1; j < stab_h->sh_size / sizeof(Stab); j++)
                    if (stab[j].n_type == N_SO || stab[j].n_type == N_FUN || stab[j].n_type == N_SLINE)
                        break;
                    else if (stab[j].n_type == N_SOL) {
                        tmp.file_name = stabstr + stab[j].n_strx;
                        break;
                    }
                lines[line_k++] = tmp;
                break;
            }
            default:
                break;
        }

    Line tmp;
    tmp.addr = ~0;
    lines[line_k++] = tmp;

    qsort(lines, line_k, sizeof(Line), line_cmp);
    qsort(funcs, func_k, sizeof(Func), func_cmp);

    uint32_t addr = 0;

    while (scanf("%x", &addr) > 0) {
        int l = 0, r = line_k;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (lines[m].addr <= addr)
                l = m;
            else
                r = m;
        }
        if (!check(funcs, func_k, addr))
            printf("0x%08x::::\n", addr);
        else {
            if (lines[l].addr <= addr && lines[l + 1].addr > addr)
                printf("0x%08x:%s:%s:%x:%d\n", addr, lines[l].file_name, lines[l].func_name, 
                    addr - lines[l].func_addr, lines[l].line);
            else
                printf("0x%08x::::\n", addr);
        }
    }

    free(lines);
    free(funcs);
    free(stab);
    free(stabstr);
    elf_close(elf_file);
}
