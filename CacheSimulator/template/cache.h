/* -*- mode:c; coding: utf-8 -*- */

#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include "abstract_memory.h"
#include "parse_config.h"
#include "random.h"

AbstractMemory *cache_create(ConfigFile *cfg, const char *var_prefix, StatisticsInfo *info, AbstractMemory *mem, Random *rnd);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/