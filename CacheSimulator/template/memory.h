/* -*- mode:c; coding: utf-8 -*- */

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "abstract_memory.h"
#include "parse_config.h"

AbstractMemory *memory_create(ConfigFile *cfg, const char *var_prefix, StatisticsInfo *info);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/