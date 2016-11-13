#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

#include "elf_common.h"

//#define DEBUG

ElfFile *
elf_open(const char *path)
{
    ElfFile *res = (ElfFile *)calloc(1, sizeof(ElfFile));

    res->fd = open(path, O_RDONLY);
    read(res->fd, &res->hdr, sizeof(Elf32_Ehdr));

    if (res->hdr.e_shnum) {
        res->sections = (Elf32_Shdr *)calloc(res->hdr.e_shnum, res->hdr.e_phentsize);
        lseek(res->fd, res->hdr.e_shoff, SEEK_SET);
        read(res->fd, res->sections, res->hdr.e_shnum * res->hdr.e_phentsize);
  
        off_t name_offset = res->sections[res->hdr.e_shstrndx].sh_offset;
        int name_size = res->sections[res->hdr.e_shstrndx].sh_size;
        lseek(res->fd, name_offset, name_size);
        res->section_names = (char *)calloc(name_size, sizeof(char));
        read(res->fd, res->section_names, name_size);
    }

    if (res->hdr.e_phnum) {
        res->segments = (Elf32_Phdr *)calloc(res->hdr.e_phnum, res->hdr.e_phentsize);
        lseek(res->fd, res->hdr.e_phoff, SEEK_SET);
        read(res->fd, res->segments, res->hdr.e_phnum * res->hdr.e_phentsize);
    }

    return res;
}

ElfFile *
elf_close(ElfFile *pelf)
{
    close(pelf->fd);
    free(pelf->sections);
    free(pelf->section_names);
    free(pelf->segments);
    free(pelf);

    return NULL;
}

const Elf32_Shdr *
elf_find_section(const ElfFile *pelf, const char *section_name)
{
    for (int i = 0; i < pelf->hdr.e_shnum; i++)
        if (!strcmp(section_name, pelf->section_names + pelf->sections[i].sh_name))
            return pelf->sections + i;
    return NULL;
}

void *
elf_load_section(const ElfFile *pelf, const Elf32_Shdr *ps)
{
    if (!ps || !pelf)
        return NULL;

    char *res = (char *)calloc(ps->sh_size, sizeof(char));
    lseek(pelf->fd, ps->sh_offset, SEEK_SET);
    read(pelf->fd, res, ps->sh_size);

    return res;
}

void *
elf_load_segment(const ElfFile *pelf, const Elf32_Phdr *ph)
{
    if (!ph || !pelf)
        return NULL;

    char *res = (char *)calloc(ph->p_filesz, sizeof(char));
    lseek(pelf->fd, ph->p_offset, SEEK_SET);
    read(pelf->fd, res, ph->p_filesz);

    return res;
}

Elf32_Nhdr *
elf_note_begin(const Elf32_Phdr *pnote, void *pdata)
{
    char *temp = (char *)pdata;
    if ((unsigned)temp % 4)
        temp += 4 - (unsigned)temp % 4;
    return (Elf32_Nhdr *)temp;
}

Elf32_Nhdr *
elf_note_end(const Elf32_Phdr *pnote, void *pdata)
{
    char *t_end = (char *)pdata + pnote->p_filesz;
    if ((unsigned)t_end % 4)
        t_end += 4 - (unsigned)t_end % 4;
    return (Elf32_Nhdr *)t_end;
}

Elf32_Nhdr *
elf_note_next(Elf32_Nhdr *pnote)
{
    char *t_note = (char *)pnote + sizeof(Elf32_Nhdr); // After PT_NOTE record header
    t_note += pnote->n_namesz; // After PT_NOTE record name
    if ((unsigned)t_note % 4)
        t_note += 4 - (unsigned)t_note % 4; // After align with 4 bytes before record data
    t_note += pnote->n_descsz; // After record data
    return (Elf32_Nhdr *)t_note;
}

const char *
elf_note_name(Elf32_Nhdr *pnote)
{
    return (char *)pnote + sizeof(Elf32_Nhdr);
}

const void *
elf_note_desc(Elf32_Nhdr *pnote)
{
    char *t_note = (char *)pnote + sizeof(Elf32_Nhdr); // After PT_NOTE record header
    t_note += pnote->n_namesz; // After PT_NOTE record name
    if ((unsigned)t_note % 4)
        t_note += 4 - (unsigned)t_note % 4; // After align with 4 bytes before record data
    return (void *)t_note;
}
