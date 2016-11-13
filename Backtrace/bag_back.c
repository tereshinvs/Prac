#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stab.h>
#include <elf.h>
#include <string.h>

#include "debug.h"

struct Stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uintptr_t n_value;
};

#define ONLINE_JUDGE

struct Stab *getFunction(unsigned char * elf, Elf32_Shdr * elf_stab, uint32_t addr )
{
    struct Stab * stab_root = (struct Stab *)(elf + elf_stab->sh_offset);
    struct Stab *lastFunction = NULL;
    int i = 0;
    for(i = 1; i < elf_stab->sh_size/sizeof(stab_root[0]); ++i) {
        if(stab_root[i].n_type == N_FUN) {
            if(lastFunction && stab_root[i].n_value > addr)
                return lastFunction;
            lastFunction = &stab_root[i];
        }
    }

    stab_root = (struct Stab *)(elf + elf_stab->sh_offset);
    for(i = 1; i < elf_stab->sh_size/sizeof(stab_root[0]); ++i) {
        if(stab_root[i].n_type == N_SO) {
            if(stab_root[i].n_value >= addr) {
                return lastFunction;
            }
        }
    }
    return NULL;
}

const char *getFunctionName(unsigned char * elf, Elf32_Shdr * elf_stab, Elf32_Shdr * elf_stabstr, uint32_t addr )
{
    struct Stab *func = getFunction(elf, elf_stab, addr);
    if(func == NULL)
        return "<unknown>";
    return (char*)(elf + elf_stabstr->sh_offset) + func->n_strx;
}

const char *getFile(unsigned char * elf, Elf32_Shdr * elf_stab, Elf32_Shdr * elf_stabstr, uint32_t addr )
{
    struct Stab *stab_root = (struct Stab *)(elf + elf_stab->sh_offset);
    int i = 0;
#ifndef ONLINE_JUDGE
    printf("debug: Total records %d, addr %x\n", elf_stab->sh_size/sizeof(stab_root[0]), addr);
#endif
    struct Stab *last_file = NULL;
    for(i = 1; i < elf_stab->sh_size/sizeof(stab_root[0]); ++i) {
        if(stab_root[i].n_type == N_SO) {
#ifndef ONLINE_JUDGE
            printf("debug: curr file: %s, lastAddr = %x\n", (char*)(elf + elf_stabstr->sh_offset) + stab_root[i].n_strx, stab_root[i].n_value);
#endif
            if(last_file && stab_root[i].n_value > addr) {
#ifndef ONLINE_JUDGE
                printf("debug MATCH file for add = %x, ind = %d, res = %s\n", addr, stab_root[i].n_strx, (char*)(elf + elf_stabstr->sh_offset) + stab_root[i].n_strx);
#endif
                return (char*)(elf + elf_stabstr->sh_offset) + last_file->n_strx;
            }
            last_file = stab_root[i].n_strx ? &(stab_root[i]) : last_file;
        }
    }
    if(getFunction(elf, elf_stab, addr))
        return (char*)(elf + elf_stabstr->sh_offset) + last_file->n_strx;
    else
        return "<unknown>";
}

int getLine(unsigned char * elf, Elf32_Shdr * elf_stab, Elf32_Shdr * elf_stabstr, uint32_t addr )
{
    struct Stab *func_root = getFunction(elf, elf_stab, addr);
    if(func_root == NULL)
        return 0;
#ifndef ONLINE_JUDGE
    printf("debug: func = %s\n", getFunctionName(elf, elf_stab, elf_stabstr, addr));
#endif
    int i = 0;
    uint32_t offset = addr - func_root->n_value;
    struct Stab *last_line = NULL;
    for(i = 1;; ++i) {
        if(func_root[i].n_type == N_SLINE) {
#ifndef ONLINE_JUDGE
            printf("debug: %x %x, delta %d, line = %d\n", func_root[i].n_value, offset, (int)(func_root[i].n_value - offset), func_root[i].n_desc);
#endif
            if( last_line && func_root[i].n_value > offset)
            {
#ifndef ONLINE_JUDGE
                printf("debug: found line %d for addr = %x\n", last_line->n_desc, addr);
#endif
                return last_line->n_desc;
            }
        }
        if(func_root[i].n_type == N_FUN)
            break;
        last_line = &func_root[i];
    }
    if(last_line)
        return last_line->n_desc;
    return 0;
}

char * stripName(const char * name)
{
    static char buf[256];
    int i = 0;
    while(name[i] && name[i] != ':') {
        buf[i] = name[i];
        ++i;
    }
    buf[i] = 0;
    return (char*)buf;
}

int max(int a,int b)
{
    return a > b ? a : b;
}

void backtrace(const char * execFileName) {
    volatile unsigned int currEbp, currEip;
    __asm("mov %%ebp, %0;" : "=r"(currEbp):);

    FILE * fElf = fopen(execFileName, "rb");
    fseek(fElf, 0L, SEEK_END);
    int elf_sz = ftell(fElf);
    fseek(fElf, 0L, SEEK_SET);
    unsigned char * elf = (unsigned char*) calloc(elf_sz, sizeof(elf[0]));
    fread(elf, elf_sz, sizeof(elf[0]), fElf);
    fclose(fElf);

    Elf32_Ehdr * hdr = (Elf32_Ehdr*) elf;
    Elf32_Shdr * names = (Elf32_Shdr*) (elf + hdr->e_shoff + hdr->e_shstrndx * hdr->e_shentsize);
    Elf32_Shdr * elf_stab = NULL;
    Elf32_Shdr * elf_stabstr = NULL;

    int i;

    for (i = 1; i < hdr->e_shnum; ++i) {
        Elf32_Shdr * curr = (Elf32_Shdr*) (elf + hdr->e_shoff + i * hdr->e_shentsize);
        char * currName = (char*)(elf + names->sh_offset) + curr->sh_name;
        if(strcmp(currName, ".stab") == 0) {
            elf_stab = curr;
        }
        if(strcmp(currName, ".stabstr") == 0) {
            elf_stabstr = curr;
        }
    }

    printf("Stack backtrace:\n");

    unsigned int stackTop = currEbp;

    while(stackTop)
    {
        if(*( (unsigned int*)stackTop ) == 0)
            break;
        stackTop = *( (unsigned int*)stackTop );
    }

    while(currEbp)
    {
        currEip = *( (unsigned int*)currEbp + 1 );
        currEbp = *( (unsigned int*)currEbp );

        if(currEbp == 0)
            break;

        unsigned int lineEip = currEip - 1;

        unsigned int outEbp = (stackTop - currEbp);

        printf("ebp 0x%08x eip 0x%08x | %s:%d: %s\n", outEbp, currEip,
               getFile(elf, elf_stab, elf_stabstr, lineEip),
               getLine(elf, elf_stab, elf_stabstr, lineEip),
               stripName(getFunctionName(elf, elf_stab, elf_stabstr, lineEip))
               );
    }

    free(elf);
}
/*
void f()
{
}

void kasjd()
{
    f();
    backtrace("/home/bagnikita/Projects/cmcmsu/main"), f(), f();
}

void asdhaskjhdkasjdh()
{
    f();
    kasjd();
    f();
}

void test2()
{
    int t = 42;
    t-=23;
    asdhaskjhdkasjdh();
}

void test1()
{
    int s = 42;
    qqqq();
}

int
main(int argc, char **argv, char **envp)
{
    test1();
    return 0;
}*/