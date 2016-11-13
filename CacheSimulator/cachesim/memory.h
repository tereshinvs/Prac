/* -*- mode:c; coding: utf-8 -*- */

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "abstract_memory.h"
#include "parse_config.h"

AbstractMemory *memory_create(ConfigFile *cfg, const char *var_prefix, StatisticsInfo *info);

int memory_get_cell(AbstractMemory *mem, int num, int *dst);

int memory_get_size(AbstractMemory *mem);

AbstractMemory *memory_free(AbstractMemory *mem);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/