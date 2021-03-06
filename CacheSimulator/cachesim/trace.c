/* -*- mode:c; coding: utf-8 -*- */

#include "trace.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum
{
    LINE_BUF_SIZE = 1024,
    MAX_LINE_LENGTH = 1000
};

struct Trace
{
    FILE *f; 
    FILE *log_f; 
    char *path; 
    int lineno; 
    TraceStep step; 
};

Trace *
trace_open(const char *path, FILE *log_f)
{
    Trace *t = (Trace*) calloc(1, sizeof(*t));
    if (!path) {
        t->path = strdup("<stdin>");
        t->f = stdin;
    } else {
        t->path = strdup(path);
        t->f = fopen(path, "r");
    }
    t->log_f = log_f;
    if (!t->f) {
        t = trace_close(t);
    }
    return t;
}

Trace *
trace_close(Trace *t)
{
    if (t) {
        if (t->f && t->f != stdin) {
            fclose(t->f);
        }
        free(t->path);
    }
    free(t); // FIXED ???
    return NULL;
}

static int
trace_error(Trace *t, const char *text)
{
    fprintf(t->log_f, "%s: %d: trace_next: %s\n", t->path, t->lineno, text);
    return -1;
}

int
trace_next(Trace *t)
{
    char buf[LINE_BUF_SIZE], *p;
    int buflen, r;
    char modes[4];
    int addr, size, n, n2;
    long long value;
 
    while (fgets(buf, sizeof(buf), t->f)) {
        ++t->lineno;
        buflen = strlen(buf);
        if (buflen > MAX_LINE_LENGTH) {
            return trace_error(t, "line is too long");
        }
        if ((p = strchr(buf, '#'))) {
            *p = 0;
            buflen = strlen(buf);
        }
        while (buflen > 0 && isspace(buf[buflen - 1])) {
            --buflen;
        }
        buf[buflen] = 0;
        if (!buflen) {
            continue;
        }

        char op, type;
        unsigned addr, size;
        long long value;
        int line_type = sscanf(buf, "%c%c %x %d %lld", &op, &type, &addr, &size, &value);
        if (line_type == 3) {
            size = 1;
            value = 0;
        }

        t->step.op = op;
        t->step.mem = type;
        t->step.addr = addr;
        t->step.size = size;

        for (int i = 0; i < size; i++, value >>= 8) {
            t->step.value[size - i - 1].value = value % 256;
            t->step.value[size - i - 1].flags = 0;
        }

        return 1;
    }
    return 0;
}

TraceStep *
trace_get(Trace *t)
{
    return &t->step;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/