#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

enum
{
    NAME_LEN = 20
};

int
elf_is_supported(Elf32_Ehdr header)
{
    if (header.e_ident[0] != '\x7f'
        || header.e_ident[1] != 'E'
        || header.e_ident[2] != 'L'
        || header.e_ident[3] != 'F'
        || (header.e_ident[9] | header.e_ident[10] | header.e_ident[11] | header.e_ident[12]
            | header.e_ident[13] | header.e_ident[14] | header.e_ident[15]))
        return -1;
    if (header.e_ident[4] != 1) // 1 for i386, 2 for my x64 mint
        return 0;
    if (header.e_ident[5] != 1) // little-endian
        return 0;
    if (header.e_ident[6] != 1) // elf version
        return 0;
    if (header.e_ident[7] != 0) // Linux elf
        return 0;
    if (header.e_ident[8] != 0)
        return 0;
    if (header.e_ehsize != sizeof(Elf32_Ehdr))
        return 0;
    if (header.e_phentsize != sizeof(Elf32_Phdr) && header.e_phentsize != 0)
        return 0;
    if (header.e_shentsize != sizeof(Elf32_Shdr) && header.e_shentsize != 0)
        return 0;
    return 1;
}

int
read_record(int fd, void *dst, int len)
{
    for (int i = 0; i < len; ) {
        int temp = read(fd, (char *)dst + i, len - i);
        if (temp <= 0)
            return 0;
        i += temp;
    }
    return 1;
}

int
load_name(int fd, char *dst, Elf32_Ehdr header, Elf32_Shdr sec_head)
{
    if (lseek(fd, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET) == -1)
        return 0;
    Elf32_Shdr temp;
    if (!read_record(fd, &temp, sizeof(temp)))
        return 0;
    char *s = (char *)calloc(temp.sh_size, 1);
    if (lseek(fd, temp.sh_offset, SEEK_SET) == -1) {
        free(s);
        return 0;
    }
    if (!read_record(fd, s, temp.sh_size) == -1) {
        free(s);
        return 0;
    }
    strcpy(dst, s + sec_head.sh_name);
    free(s);
    return 1;
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Couldn't open input file\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Couldn't open input file\n");
        return 1;
    }

    Elf32_Ehdr header;
    if (!read_record(fd, &header, sizeof(header))) {
        fprintf(stderr, "Error while reading ELF Header\n");
        close(fd);
        return 1;
    }
    int err_code = elf_is_supported(header);
    if (err_code == 0) {
        fprintf(stderr, "Not supported ELF file\n");
        close(fd);
        return 1;
    }
    if (err_code == -1) {
        fprintf(stderr, "Not an ELF file\n");
//        fprintf(stderr, "Error while reading ELF Header\n");
        close(fd);
        return 1;
    }

    printf("TYPE: ");
    switch (header.e_type) {
        case 0:
            printf("NONE\n");
            break;
        case 1:
            printf("REL\n");
            break;
        case 2:
            printf("EXEC\n");
            break;
        case 3:
            printf("DYN\n");
            break;
        case 4:
            printf("CORE\n");
            break;
        default:
            fprintf(stderr, "Not an ELF file\n");
            close(fd);
            return 1;
            break;
    }

    printf("                NAME       ADDR     OFFSET       SIZE   ALGN\n");
    Elf32_Shdr sec_head;
    char sec_name[NAME_LEN];
    for (int i = 1; i < header.e_shnum; i++) {
        if (lseek(fd, header.e_shoff + i * header.e_shentsize, SEEK_SET) == -1) {
            fprintf(stderr, "Not an ELF file\n");
            close(fd);
            return 1;
        }
        if (!read_record(fd, &sec_head, sizeof(sec_head))) {
            fprintf(stderr, "Not an ELF file\n");
            close(fd);
            return 1;
        }
        if (!load_name(fd, sec_name, header, sec_head)) {
            fprintf(stderr, "Not an ELF file\n");
            close(fd);
            return 1;
        }
        printf("%20s 0x%08x %10d %10d 0x%04x\n", sec_name,
            sec_head.sh_addr, sec_head.sh_offset, sec_head.sh_size, sec_head.sh_addralign);
    }

    close(fd);
    fclose(stdout);
    return 0;
}
