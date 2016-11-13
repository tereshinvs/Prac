/* -*- mode:c; coding: utf-8 -*- */

#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include "common.h"

#include <stdio.h>
 
typedef struct TraceStep
{
    char op; 
    char mem; 
    memaddr_t addr; 
    int size; 
    MemoryCell value[8]; 
} TraceStep;

struct Trace;
typedef struct Trace Trace;

Trace *trace_open(const char *path, FILE *log_f);

Trace *trace_close(Trace *t);

int trace_next(Trace *t);

TraceStep *trace_get(Trace *t);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/