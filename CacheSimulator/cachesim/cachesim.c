/* -*- mode:c; coding: utf-8 -*- */

#include "parse_config.h"
#include "statistics.h"
#include "abstract_memory.h"
#include "memory.h"
#include "random.h"
#include "cache.h"
#include "trace.h"

#include <stdio.h>
#include <string.h>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

enum
{
	MAX_LINE_LENGTH = 1000,
	LINE_BUF_SIZE = 1024
};

enum
{
	SHORT_TRACE_RECORD = 3,
	FULL_TRACE_RECORD = 5
};

int
is_there_param(int argc, char *argv[], const char *param_name)
{
    for (int i = 0; i < argc; i++)
        if (!strcmp(param_name, argv[i]))
            return i;
    return -1;
}

void
print_conf_params(const ConfigFile *cfg)
{
    int len = config_file_length(cfg);
    for (int i = 0; i < len; i++) {
        const char *curname = config_file_get_nth_name(cfg, i);
        printf("%s = \"%s\"\n", curname, config_file_get(cfg, curname));
    }
}

void
out_hexadecimal(unsigned char x)
{
	int tmp = x / 16;
	if (tmp < 10)
		printf("%c", tmp + '0');
	else
		printf("%c", tmp - 10 + 'A');
	tmp = x % 16;
	if (tmp < 10)
		printf("%c", tmp + '0');
	else
		printf("%c", tmp - 10 + 'A');
}

int
main(int argc, char *argv[])
{
    // Код запуска симулятора
    if (argc < 2) {
    	fprintf(stderr, "Invalid arguments\n");
    	return EXIT_FAILURE;
    }

    ConfigFile *cfg = config_file_parse(argv[argc - 1], stderr);
    if (!cfg)
    	return EXIT_FAILURE;

    if (is_there_param(argc, argv, "--print-config") != -1) {
    	print_conf_params(cfg);
    	config_file_free(cfg);
    	return 0;
    }

    StatisticsInfo *info = statistics_create(cfg);
    if (!info)
    	return EXIT_FAILURE;
    AbstractMemory *mem = memory_create(cfg, "", info);
    if (!mem)
    	return EXIT_FAILURE;
    Random *rnd = random_create(cfg);
    if (!rnd)
    	return EXIT_FAILURE;

    if (is_there_param(argc, argv, "--disable-cache") == -1)
    	mem = cache_create(cfg, "", info, mem, rnd);

    Trace *trace = trace_open(NULL, stderr);
    TraceStep *step;

    if (!trace)
    	return EXIT_FAILURE;

    int r = 0;
    while (r = trace_next(trace)) {
    	if (r < 0) {
    		trace_close(trace);
		    memory_free(mem);
    		random_free(rnd);
    		statistics_free(info);
    		config_file_free(cfg);
            return EXIT_FAILURE;
    	}
    	step = trace_get(trace);

    	if (step->op == 'R')
    		mem->ops->read(mem, step->addr, step->size, step->value);
    	else
    		mem->ops->write(mem, step->addr, step->size, step->value);
    }
    trace_close(trace);

    if (is_there_param(argc, argv, "--dump-memory") != -1) {
    	for (int i = 0; i < memory_get_size(mem); i++) {
    		if (i % 16 == 0)
    			printf("%08X ", i);
    		int tmp;
    		if (memory_get_cell(mem, i, &tmp) == 0)
    			printf("??");
    		else
    			out_hexadecimal(tmp);
    		if (i % 16 == 15)
    			printf("\n");
    		else
    			printf(" ");
    	}
    }

    if (is_there_param(argc, argv, "--statistics") != -1) {
    	printf("clock count: %d\n", info->clock_counter);
    	printf("reads: %d\n", info->read_counter);
    	printf("writes: %d\n", info->write_counter);
//    	printf("read hits: %d\n", info->hit_counter);
//    	printf("cache blocks writes: %d\n", 0); // FIXME
    }

    memory_free(mem);
    random_free(rnd);
    statistics_free(info);
    config_file_free(cfg);

    return 0;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/