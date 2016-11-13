/* -*- mode:c; coding: utf-8 -*- */

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include "parse_config.h"

struct Random;
typedef struct Random Random;

typedef struct RandomOps
{
    Random *(*free)(Random *rnd);
    int (*next)(Random *rnd, int n);
} RandomOps;

Random *random_create(ConfigFile *cfg);

Random *random_free(Random *rnd);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/