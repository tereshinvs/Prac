/* -*- mode:c; coding: utf-8 -*- */

#ifndef FULL_CACHE_H_INCLUDED
#define FULL_CACHE_H_INCLUDED

#include "abstract_memory.h"
#include "parse_config.h"
#include "random.h"

AbstractMemory *full_cache_create(ConfigFile *cfg, const char *var_prefix, StatisticsInfo *info, AbstractMemory *mem, Random *rnd);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/