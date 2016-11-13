#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include "elf_common.h"

int
main(int argc, char *argv[])
{
    ElfFile *elf_file = elf_open(argv[1]);

    unsigned addr = 0;
    while (scanf("%x", &addr) == 1) {
    	int smth_wrt = 0;

        for (int i = 0; i < elf_file->hdr.e_phnum; i++) {
            if (elf_file->segments[i].p_type != PT_LOAD)
                continue;

            if (elf_file->segments[i].p_vaddr <= addr
                && addr < elf_file->segments[i].p_vaddr + elf_file->segments[i].p_memsz) {

                if (elf_file->segments[i].p_filesz > 0
                    && elf_file->segments[i].p_filesz == elf_file->segments[i].p_memsz) {

                    unsigned res = 0;
                    lseek(elf_file->fd,
                    	elf_file->segments[i].p_offset + addr - elf_file->segments[i].p_vaddr,
                    	SEEK_SET);
                    read(elf_file->fd, &res, sizeof(unsigned));
                    printf("0x%08x 0x%08x\n", addr, res);

                } else
                    printf("0x%08x UNKNOWNVAL\n", addr);

                smth_wrt = 1;
                break;
            }
        }

        if (!smth_wrt)
            printf("0x%08x INVALIDADR\n", addr);
    }

    elf_file = elf_close(elf_file);
    return 0;
}
