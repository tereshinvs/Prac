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

#define new(type) (type *) calloc(1, sizeof(type))
#define newarr(type, count) (type *) malloc((count) * sizeof(type))

typedef struct ElfFile {
    int fd;               // файловый дескриптор доступа к файлу
    Elf32_Ehdr *hdr;      // ELF-заголовок файла
    Elf32_Shdr *sections; // массив секции, NULL - если секций нет
    char *section_names;  // содержимое секции имен секций
    Elf32_Phdr *segments; // массив заголовков программ, NULL - если нет
} ElfFile;

typedef struct Stab {
    uint32_t n_strx;   // позиция начала строки в секции .strstab
    uint8_t n_type;    // тип отладочного символа
    uint8_t n_other;   // прочая информация
    uint16_t n_desc;   // описание отладочного символа
    uint32_t n_value; // значение отладочного символа
} Stab;

ElfFile *elf_open(const char *path);
ElfFile *elf_close(ElfFile *pelf);
const Elf32_Shdr *elf_find_section(const ElfFile *pelf, const char *section_name);
void *elf_load_section(const ElfFile *pelf, const Elf32_Shdr *ps);
void *elf_load_segment(const ElfFile *pelf, const Elf32_Phdr *ph);

Elf32_Nhdr *elf_note_begin(const Elf32_Phdr *pnote, void *pdata);
Elf32_Nhdr *elf_note_end(const Elf32_Phdr *pnote, void *pdata);
Elf32_Nhdr *elf_note_next(Elf32_Nhdr *pnote);
const char *elf_note_name(Elf32_Nhdr *pnote);
const void *elf_note_desc(Elf32_Nhdr *pnote);

ElfFile* elf_open(const char *path) {
    ElfFile *res = new(ElfFile);
    // Open file
    res->fd = open(path, O_RDONLY);
    if (!res->fd) {
        return NULL;
    }
    // Read main hdr
    res->hdr = new(Elf32_Ehdr);
    read(res->fd, res->hdr, sizeof *res->hdr);
    // Read section hdrs
    if (!res->hdr->e_shnum) {
        res->sections = NULL;
    } else {
        res->sections = newarr(Elf32_Shdr, res->hdr->e_shnum);
        lseek(res->fd, res->hdr->e_shoff, SEEK_SET);
        read(res->fd, res->sections, res->hdr->e_shnum * sizeof(Elf32_Shdr));
        // Read section names
        res->section_names = elf_load_section(res, res->sections + res->hdr->e_shstrndx);
    }
    // Read program hdrs
    if (!res->hdr->e_phnum) {
        res->segments = NULL;
    } else {
        res->segments = newarr(Elf32_Phdr, res->hdr->e_phnum);
        lseek(res->fd, res->hdr->e_phoff, SEEK_SET);
        read(res->fd, res->segments, res->hdr->e_phnum * sizeof(Elf32_Phdr));
    }
    return res;
}

ElfFile *elf_close(struct ElfFile *pelf) {
    // Close file
    close(pelf->fd);
    // Free resources
    free(pelf->hdr);
    free(pelf->section_names);
    if (pelf->sections) {
        free(pelf->sections);
    }
    if (pelf->segments) {
        free(pelf->segments);
    }
    // And free structure itself
    free(pelf);
    return NULL;
}

const Elf32_Shdr *elf_find_section(const ElfFile *pelf, const char *section_name) {
    for (int i = 0; i < pelf->hdr->e_shnum; ++i) {
        if (!strcmp(section_name, pelf->section_names + pelf->sections[i].sh_name)) {
            return &pelf->sections[i];
        }
    }
    return NULL;
}

void *elf_load_section(const ElfFile *pelf, const Elf32_Shdr *ps) {
    void *res = malloc(ps->sh_size);
    if (!res) {
        return NULL;
    }
    lseek(pelf->fd, ps->sh_offset, SEEK_SET);
    if (read(pelf->fd, res, ps->sh_size) != ps->sh_size) {
        free(res);
        return NULL;
    }
    return res;
}

void *elf_load_segment(const ElfFile *pelf, const Elf32_Phdr *ph) {
    void *res = malloc(ph->p_filesz);
    if (!res) {
        return NULL;
    }
    lseek(pelf->fd, ph->p_offset, SEEK_SET);
    if (read(pelf->fd, res, ph->p_filesz) != ph->p_filesz) {
        free(res);
        return NULL;
    }
    return res;
}

Elf32_Nhdr *elf_note_begin(const Elf32_Phdr *pnote, void *pdata) {
    Elf32_Nhdr *res = (Elf32_Nhdr *)pdata;
    return res;
}

Elf32_Nhdr *elf_note_end(const Elf32_Phdr *pnote, void *pdata) {
    Elf32_Nhdr *res = (Elf32_Nhdr *)pdata + pnote->p_filesz;
    return res;
}

void trim(char *s) {
    for (; *s; ++s) {
        if (*s == ':') {
            *s = '\0';
            break;
        }
    }
}

#define min(a, b) ((a) < (b) ? a : b)
#define max(a, b) ((a) > (b) ? a : b)

typedef struct Line {
    int line;
    uint32_t addr;
    uint32_t func_addr;
    char *file_name;
    char *func_name;
} Line;

typedef struct Func {
    uint32_t begin, end;
    char *name;
} Func;

int l_cmp(Line *a, Line *b) {
    if (a->addr < b->addr) {
        return -1;
    } else if (a->addr > b->addr) {
        return 1;
    } else {
        return 0;
    }
}

int f_cmp(Func *a, Func *b) {
    if (a->begin < b->begin) {
        return -1;
    } else if (a->begin > b->begin) {
        return 1;
    } else {
        return 0;
    }
}

int check(Func *a, int n, uint32_t addr) {
    int l = 0;
    int r = n;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (a[m].begin <= addr) {
            l = m;
        } else {
            r = m;
        }
    }
    return a[l].begin <= addr && addr < a[l].end;
}

typedef struct Vector_f {
    int allocated, size;
    Func *data;
    void (*push_back) (struct Vector_f *this, Func v);
} Vector_f;

void Vector_f__push_back(struct Vector_f *this, Func v);

Vector_f *new_Vector_f(void) {
    Vector_f *res = new(Vector_f);
    res->push_back = Vector_f__push_back;
    res->allocated = 10;
    res->data = (Func *) malloc(res->allocated * sizeof(Func));
    return res;
}

void delete_Vector_f(Vector_f *this) {
    free(this->data);
    free(this);
}

void Vector_f__push_back(struct Vector_f *this, Func v) {
    if (this->allocated == this->size) {
        this->data = (Func *) realloc(this->data, sizeof(Func) * (this->allocated *= 2));
    }
    this->data[this->size++] = v;
}

typedef struct Vector_l {
    int allocated, size;
    Line *data;
    void (*push_back) (struct Vector_l *this, Line v);
} Vector_l;

void Vector_l__push_back(struct Vector_l *this, Line v);

Vector_l *new_Vector_l(void) {
    Vector_l *res = new(Vector_l);
    res->push_back = Vector_l__push_back;
    res->allocated = 10;
    res->data = (Line *) malloc(res->allocated * sizeof(Line));
    return res;
}

void delete_Vector_l(Vector_l *this) {
    free(this->data);
    free(this);
}

void Vector_l__push_back(struct Vector_l *this, Line v) {
    if (this->allocated == this->size) {
        this->data = (Line *) realloc(this->data, sizeof(Line) * (this->allocated *= 2));
    }
    this->data[this->size++] = v;
}

int main(int argc, char *argv[]) {
    ElfFile *pElf = elf_open(argv[1]);

    const Elf32_Shdr *stab_h = elf_find_section(pElf, ".stab");
    const Elf32_Shdr *stabstr_h = elf_find_section(pElf, ".stabstr");

    if (!stab_h || !stabstr_h) {
        printf("No debug info\n");
        elf_close(pElf);
        return 0;
    }

    Vector_l *lines = new_Vector_l();
    Vector_f *funcs = new_Vector_f();

    char *cur_file = NULL;
    char *cur_func = NULL;
    uint32_t cur_func_addr = 0;
    Stab *stab = elf_load_section(pElf, stab_h);
    char *stabstr = elf_load_section(pElf, stabstr_h);

    int i = 0;
    for (Stab *is = stab; i < stab_h->sh_size / sizeof(Stab); ++i, ++is) {
        switch (is->n_type) {
        case N_SO:
            if (funcs->size) {
                if (funcs->data[funcs->size - 1].end > is->n_value) {
                    funcs->data[funcs->size - 1].end = is->n_value;
                }
            }
            cur_file = stabstr + is->n_strx;
            break;
        case N_SOL:
            cur_file = stabstr + is->n_strx;
            break;
        case N_FUN: {
            cur_func = stabstr + is->n_strx;
            trim(cur_func);
            cur_func_addr = is->n_value;
            
            Func tmp;
            tmp.begin = cur_func_addr;
            tmp.end = ~0;
            tmp.name = cur_func;
            if (funcs->size) {
                funcs->data[funcs->size - 1].end = min(funcs->data[funcs->size - 1].end, cur_func_addr);
            }
            funcs->push_back(funcs, tmp);
            break;
        }
        case N_SLINE: {
            Line tmp;
            tmp.line = is->n_desc;
            tmp.addr = cur_func_addr + is->n_value;
            tmp.func_name = cur_func;
            tmp.func_addr = cur_func_addr;
            tmp.file_name = cur_file;
            for (int j = i + 1; j < stab_h->sh_size / sizeof(Stab); ++j) {
                if (stab[j].n_type == N_SO || stab[j].n_type == N_FUN || stab[j].n_type == N_SLINE) {
                    break;
                } else if (stab[j].n_type == N_SOL) {
                    tmp.file_name = stabstr + stab[j].n_strx;
                    break;
                }
            }
            lines->push_back(lines, tmp);
            break;
        }
        default:
            break;
        }
    }

    Line tmp;
    tmp.addr = ~0;
    lines->push_back(lines, tmp);

    qsort(lines->data, lines->size, sizeof(Line), (__compar_fn_t) l_cmp);
    qsort(funcs->data, funcs->size, sizeof(Func), (__compar_fn_t) f_cmp);

    uint32_t addr = 0;

    while (scanf("%x", &addr) > 0) {
        int l = 0;
        int r = lines->size;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (lines->data[m].addr <= addr) {
                l = m;
            } else {
                r = m;
            }
        }
        if (!check(funcs->data, funcs->size, addr)) {
            printf("0x%08x::::\n", addr);
        } else {
            if (lines->data[l].addr <= addr && lines->data[l + 1].addr > addr) {
                printf("0x%08x:%s:%s:%x:%d\n", addr, lines->data[l].file_name, lines->data[l].func_name, 
                                                                addr - lines->data[l].func_addr, lines->data[l].line);
            } else {
                printf("0x%08x::::\n", addr);
            }
        }
    }

    delete_Vector_f(funcs);
    delete_Vector_l(lines);
    free(stab);
    free(stabstr);
    elf_close(pElf);
}
