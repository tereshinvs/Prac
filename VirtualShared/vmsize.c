#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>

#include "vmarea.h"

int main(int argc, char *argv[]) {
    long long virt = 0, phys = 0, unsh = 0;
    VirtualMapping *map = mem_mapping_create(argc - 1, argv + 1);
    if (!map->size) {
        mem_mapping_free(map);
        printf("0\n0\n0\n");
        return 0;
    }

    qsort(map->data, map->size, sizeof(map->data[0]), mem_mapping_cmp);

    for (int i = 0; i < map->size - 1; i++) {
        virt += map->data[i].size;
        if (!is_shared(map->data + i, map->data + i + 1))
            phys += map->data[i].size;
        if (!is_shareable(map->data + i))
            unsh += map->data[i].size;
    }

    virt += map->data[map->size - 1].size;
    phys += map->data[map->size - 1].size;
    if (!is_shareable(map->data + map->size - 1))
        unsh += map->data[map->size - 1].size;

    printf("%llu %llu %llu\n", virt, phys, unsh);
    
    mem_mapping_free(map);
    return 0;
}
