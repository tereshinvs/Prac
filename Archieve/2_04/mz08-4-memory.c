#include <stdio.h>
#include <stdlib.h>

enum
{
    MAX_MEMORY_SIZE = 1024 * 1024
};

void
out_byte(char _t)
{
    unsigned char t = (unsigned char)_t;
    unsigned char fst_smb = t / 16;
    unsigned char scd_smb = t % 16;

    if (fst_smb < 10)
        printf("%c", fst_smb + '0');
    else
        printf("%c", fst_smb - 10 + 'A');

    if (scd_smb < 10)
        printf("%c", scd_smb + '0');
    else
        printf("%c", scd_smb - 10 + 'A');

    printf(" ");
}

int
main(int argc, char *argv[])
{
    int memory_size = 0;
    sscanf(argv[1], "%d", &memory_size);

    char *memory = (char *)calloc(MAX_MEMORY_SIZE, sizeof(char));
    char *block_cntn_smth = (char *)calloc(MAX_MEMORY_SIZE, sizeof(char));

    char type = 0, inst_data = 0;
    unsigned addr = 0;
    int size = 0, value = 0;
    while (scanf("%c%c %x %d %d\n", &type, &inst_data, &addr, &size, &value) == 5)
        for (int i = 0; i < size; i++) {
            memory[addr + size - i - 1] = value % 256;
            block_cntn_smth[addr + size - i - 1] = 1;
            value >>= 8;
        }

    for (int i = 0; i < memory_size / 16; i++) {
        printf("%08x ", i * 16);
        for (int j = 0; j < 16; j++)
            if (block_cntn_smth[i * 16 + j])
                out_byte(memory[i * 16 + j]);
            else
                printf("?? ");
        printf("\n");
    }

    free(memory);
    free(block_cntn_smth);
    return 0;
}
