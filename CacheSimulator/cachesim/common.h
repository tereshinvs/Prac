/* -*- mode:c; coding: utf-8 -*- */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

typedef int memaddr_t;

typedef struct MemoryCell
{
    unsigned char value; 
    unsigned char flags; 
} MemoryCell;

enum
{
    KiB = 1024,
    MiB = 1024 * 1024,
    GiB = 1024 * 1024 * 1024
};

enum
{
    MAX_ADDRESS = GiB 
};


char *make_param_name(char *buf, int size, const char *prefix, const char *name);

void error_undefined(const char *func, const char *param);

void error_invalid(const char *func, const char *param);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/