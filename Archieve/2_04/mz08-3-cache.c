#include <stdio.h>

int
main(int argc, char *argv[])
{
    int memory_size, cache_size, block_size;
    sscanf(argv[1], "%d", &memory_size);
    sscanf(argv[2], "%d", &cache_size);
    sscanf(argv[3], "%d", &block_size);

    int block_count = cache_size / block_size;
    unsigned addr_in_cache[block_count];
    char block_cntn_smth[block_count];

    for (int i = 0; i < block_count; i++)
        block_cntn_smth[i] = 0;

    unsigned req_count = 0, bad_req = 0;

    char type = 0, inst_data = 0;
    unsigned addr = 0;
    int size = 0, value = 0;
    while (scanf("%c%c %x %d %d\n", &type, &inst_data, &addr, &size, &value) == 5) {
        int mem_block_num = addr / block_size;
        int cache_block_num = mem_block_num % block_count;

        if (block_cntn_smth[cache_block_num] && addr_in_cache[cache_block_num] != mem_block_num)
            bad_req++;

        addr_in_cache[cache_block_num] = mem_block_num;
        block_cntn_smth[cache_block_num] = 1;

        req_count++;
    }

    printf("%u\n", bad_req);

    return 0;
}
