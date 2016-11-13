typedef struct ElfFile
{
    int fd;
    Elf32_Ehdr hdr;
    Elf32_Shdr *sections;
    char *section_names;
    Elf32_Phdr *segments;
} ElfFile;

ElfFile *elf_open(const char *);

ElfFile *elf_close(ElfFile *);

const Elf32_Shdr *elf_find_section(const ElfFile *, const char *);

void *elf_load_section(const ElfFile *, const Elf32_Shdr *);

void *elf_load_segment(const ElfFile *, const Elf32_Phdr *);

Elf32_Nhdr *elf_note_begin(const Elf32_Phdr *, void *);

Elf32_Nhdr *elf_note_end(const Elf32_Phdr *, void *);

Elf32_Nhdr *elf_note_next(Elf32_Nhdr *);

const char *elf_note_name(Elf32_Nhdr *);

const void *elf_note_desc(Elf32_Nhdr *);