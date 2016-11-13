#include <stdlib.h>
#include <stdio.h>

#include "vmarea.h"

int
mem_mapping_read_file(FILE *in, VirtualMemory *dst)
{
    return fscanf(in, "%x-%x%s%x%x:%x%d", &dst->start, &dst->end, dst->permissions,
        &dst->offset, &dst->dev_maj, &dst->dev_min, &dst->desc);
}

VirtualMapping *
mem_mapping_create(int argc, char *argv[])
{
	VirtualMapping *dst = (VirtualMapping *)calloc(1, sizeof(VirtualMapping));
    dst->allocated = ALLOCATING_START_VALUE;
    dst->size = 0;
    dst->data = (VirtualMemory *)calloc(dst->allocated, sizeof(VirtualMemory));

	for (int i = 0; i < argc; i++) {
	    FILE *in = fopen(argv[i], "r");
    	while (1) {
    	    if (dst->size == dst->allocated) {
    	        dst->allocated *= ALLOCATING_FACTOR;
    	        dst->data = (VirtualMemory *)realloc(dst->data, dst->allocated * sizeof(VirtualMemory));
    	    }
    	    if (mem_mapping_read_file(in, &dst->data[dst->size]) < 6)
    	        break;
    	    else {
    	        if (dst->data[dst->size].permissions[0] == 'r') {
					dst->data[dst->size].size = dst->data[dst->size].end - dst->data[dst->size].start;
	    	        dst->size++;
				}
    	        while (fgetc(in) != '\n');
    	    }
    	}
	    fclose(in);
	}
    return dst;
}

void
mem_mapping_free(VirtualMapping *map)
{
    free(map->data);
    free(map);
}

int
is_shareable(VirtualMemory *mem)
{
    int ok = mem->permissions[1] == 'w' || mem->permissions[3] != 'p'
    	|| !mem->desc || (!mem->dev_maj && !mem->dev_min);
	return !ok;
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

int
mem_mapping_cmp(const void *_v1, const void *_v2)
{
    VirtualMemory *v1 = (VirtualMemory *)_v1;
    VirtualMemory *v2 = (VirtualMemory *)_v2;
    if (v1->desc == v2->desc) {
        if (v1->offset == v2->offset) {
            if (v1->size == v2->size)
                return v1->start > v2->start ? 1 : -1;
            else
                return v1->size > v2->size ? 1 : -1;
        } else
            return v1->offset > v2->offset ? 1 : -1;
    } else
        return v1->desc > v2->desc ? 1 : -1;
}

