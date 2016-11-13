/* -*- mode:c; coding: utf-8 -*- */

#ifndef ABSTRACT_MEMORY_H_INCLUDED
#define ABSTRACT_MEMORY_H_INCLUDED

#include "common.h"
#include "statistics.h"

struct AbstractMemory;
typedef struct AbstractMemory AbstractMemory;

typedef struct AbstractMemoryOps
{
    AbstractMemory *(*free)(AbstractMemory *m);
    void (*read)(AbstractMemory *m, memaddr_t addr, int size, MemoryCell *dst);
    void (*write)(AbstractMemory *m, memaddr_t addr, int size, const MemoryCell *src);
    void (*reveal)(AbstractMemory *m, memaddr_t addr, int size, const MemoryCell *src);
    void (*flush)(AbstractMemory *m);
} AbstractMemoryOps;

struct AbstractMemory
{
    AbstractMemoryOps *ops; 
    StatisticsInfo *info; 
};

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/