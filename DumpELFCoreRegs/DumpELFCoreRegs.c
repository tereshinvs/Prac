#include <stdio.h>
#include <unistd.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/procfs.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//#define DEBUG

typedef struct ElfFile
{
    int fd;
    Elf32_Ehdr hdr;
    Elf32_Shdr *sections;
    char *section_names;
    Elf32_Phdr *segments;
} ElfFile;

ElfFile *
elf_open(const char *path)
{
    ElfFile *res = (ElfFile *)calloc(1, sizeof(ElfFile));

    res->fd = open(path, O_RDONLY);
    read(res->fd, &res->hdr, sizeof(Elf32_Ehdr));

    res->sections = (Elf32_Shdr *)calloc(res->hdr.e_shnum, res->hdr.e_shentsize);
    lseek(res->fd, res->hdr.e_shoff, SEEK_SET);
    read(res->fd, res->sections, res->hdr.e_shnum * res->hdr.e_shentsize);

    off_t name_offset = res->sections[res->hdr.e_shstrndx].sh_offset;
    int name_size = res->sections[res->hdr.e_shstrndx].sh_size;
    lseek(res->fd, name_offset, name_size);
    res->section_names = (char *)calloc(name_size, sizeof(char));
    read(res->fd, res->section_names, name_size);

    res->segments = (Elf32_Phdr *)calloc(res->hdr.e_phnum, res->hdr.e_phentsize);
    lseek(res->fd, res->hdr.e_phoff, SEEK_SET);
    read(res->fd, res->segments, res->hdr.e_phnum * res->hdr.e_phentsize);

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

void
elf_dump_regs(FILE *out, const char *path)
{
    ElfFile *elf_file = elf_open(path);

    #ifdef DEBUG
        printf("File opened\n");
        fflush(stdout);
    #endif

    Elf32_Phdr *pnote = NULL;
    for (int i = 0; i < elf_file->hdr.e_phnum; i++)
        if (elf_file->segments[i].p_type == PT_NOTE) {
            pnote = elf_file->segments + i;
            break;
        }

    #ifdef DEBUG
        printf("PT_NOTE section found at 0x%08x\n", pnote);
        fflush(stdout);
    #endif

    void *pdata = elf_load_segment(elf_file, pnote);
    Elf32_Nhdr *fnote = elf_note_begin(pnote, pdata);
    Elf32_Nhdr *lnote = elf_note_end(pnote, pdata);

    #ifdef DEBUG
        printf("First and last PT_NOTE record found at 0x%08x and 0x%08x\n", fnote, lnote);
        fflush(stdout);
    #endif


    Elf32_Nhdr *regs_rec = NULL;
    for (Elf32_Nhdr *i = fnote; i < lnote; i = elf_note_next(i))
        if (i->n_type == NT_PRSTATUS) {
            regs_rec = i;
            break;
        }

    #ifdef DEBUG
        printf("Proc status record found at 0x%08x\n", regs_rec);
        fflush(stdout);
    #endif

    prstatus_t *proc_st = (prstatus_t *)elf_note_desc(regs_rec);

    #ifdef DEBUG
        printf("%d\n", ELF_NGREG);
        fflush(stdout);
    #endif

    fprintf(out, "ebx:    0x%08x\n", (unsigned)proc_st->pr_reg[0]);
    fprintf(out, "ecx:    0x%08x\n", (unsigned)proc_st->pr_reg[1]);
    fprintf(out, "edx:    0x%08x\n", (unsigned)proc_st->pr_reg[2]);
    fprintf(out, "esi:    0x%08x\n", (unsigned)proc_st->pr_reg[3]);
    fprintf(out, "edi:    0x%08x\n", (unsigned)proc_st->pr_reg[4]);
    fprintf(out, "ebp:    0x%08x\n", (unsigned)proc_st->pr_reg[5]);
    fprintf(out, "eax:    0x%08x\n", (unsigned)proc_st->pr_reg[6]);
    fprintf(out, " ds:    0x%04x\n", (unsigned)proc_st->pr_reg[7]);
    fprintf(out, " es:    0x%04x\n", (unsigned)proc_st->pr_reg[8]);
    fprintf(out, " fs:    0x%04x\n", (unsigned)proc_st->pr_reg[9]);
    fprintf(out, " gs:    0x%04x\n", (unsigned)proc_st->pr_reg[10]);
    fprintf(out, "eip:    0x%08x\n", (unsigned)proc_st->pr_reg[12]);
    fprintf(out, " cs:    0x%04x\n", (unsigned)proc_st->pr_reg[13]);
    fprintf(out, "eflags: 0x%08x\n", (unsigned)proc_st->pr_reg[14]);
    fprintf(out, "esp:    0x%08x\n", (unsigned)proc_st->pr_reg[15]);
    fprintf(out, " ss:    0x%04x\n", (unsigned)proc_st->pr_reg[16]);
    fflush(out);

    free(pdata);
    elf_file = elf_close(elf_file);
}

#ifdef DEBUG
int main(int argc, char *argv[]) {
    elf_dump_regs(stdout, argv[1]);

    return 0;
}
#endif
