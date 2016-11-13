#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

enum
{
    ALLOCATING_START_VALUE = 10,
    ALLOCATING_FACTOR = 2
};

typedef struct VirtualMemory
{
    uint32_t start, end, size;
    uint32_t offset;
    uint32_t dev_maj, dev_min;
    uint32_t desc;
    char permissions[5];
} VirtualMemory;

typedef struct VirtualMapping
{
    VirtualMemory *data;
    uint32_t allocated;
    uint32_t size;
} VirtualMapping;

void
free_mapping(VirtualMapping *map)
{
    free(map->data);
    free(map);
}

VirtualMapping *
new_mapping(char *path)
{
    VirtualMapping *res = (VirtualMapping *)calloc(1, sizeof(VirtualMapping));
    res->allocated = ALLOCATING_START_VALUE;
    res->size = 0;
    res->data = (VirtualMemory *)calloc(res->allocated, sizeof(VirtualMemory));

    FILE *in = fopen(path, "r");
    while (1) {
        if (res->size == res->allocated) {
            res->allocated *= ALLOCATING_FACTOR;
            res->data = (VirtualMemory *)realloc(res->data, res->allocated * sizeof(VirtualMemory));
        }
        if (fscanf(in, "%x-%x%s%x%x:%x%d", &res->data[res->size].start, &res->data[res->size].end, 
            res->data[res->size].permissions, &res->data[res->size].offset, &res->data[res->size].dev_maj,
            &res->data[res->size].dev_min, &res->data[res->size].desc) < 6)
            break;
        else {
            res->data[res->size].size = res->data[res->size].end - res->data[res->size].start;
            ++res->size;
            while (fgetc(in) != '\n');
        }
    }
    fclose(in);
    return res;
}

int
is_shared(VirtualMemory *m1, VirtualMemory *m2)
{
    return m1->size == m2->size && m1->offset == m2->offset
        && m1->permissions[0] == 'r' && m1->permissions[1] == '-'
        && m2->permissions[0] == 'r' && m2->permissions[1] == '-'
        && m1->permissions[3] == 'p' && m2->permissions[3] == 'p'
        && m1->dev_maj && m1->dev_min
        && m2->dev_maj && m2->dev_min
        && m1->dev_maj == m2->dev_maj && m1->dev_min == m2->dev_min
        && m1->desc && m2->desc && (m1->desc == m2->desc);
}

int *
remove_alr_used(VirtualMapping *map)
{
    int *used = (int *)calloc(map->size, sizeof(int));
    for (int i = 0; i < map->size; ++i)
        for (int j = i + 1; j < map->size; ++j)
            if (!used[i] && is_shared(map->data + i, map->data + j))
                used[j] = 1;
    return used;
}

int
main(int argc, char *argv[])
{
    VirtualMapping *map1 = new_mapping(argv[1]);
    VirtualMapping *map2 = new_mapping(argv[2]);

    long long res = 0;

    int *used1 = remove_alr_used(map1);
    int *used2 = remove_alr_used(map2);

    for (int i = 0; i < map1->size; ++i) {
        if (used1[i])
            continue;
        for (int j = 0; j < map2->size; ++j) {
            if (used2[j])
                continue;
            if (is_shared(map1->data + i, map2->data + j)) {
                res += map1->data[i].size;
                break;
            }
        }
    }
    printf("%lld\n", res);

    free_mapping(map1);
    free_mapping(map2);
    free(used1);
    free(used2);
    return 0;
}
