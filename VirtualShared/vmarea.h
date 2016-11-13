#ifndef _VMAREA_H_
#define _VMAREA_H_

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

VirtualMapping *mem_mapping_create(int argc, char *argv[]);

void mem_mapping_free(VirtualMapping *map);

int is_shareable(VirtualMemory *mem);
int is_shared(VirtualMemory *m1, VirtualMemory *m2);

int mem_mapping_cmp(const void *_v1, const void *_v2);

#endif
