/* -*- mode:c; coding: utf-8 -*- */

#ifndef STATISTICS_H_INCLUDED
#define STATISTICS_H_INCLUDED

#include "parse_config.h"

#include <stdio.h>

struct StatisticsInfo;
typedef struct StatisticsInfo StatisticsInfo;

struct StatisticsInfo
{
    int hit_counter_needed; 
    int write_back_needed; 
    int clock_counter; 
    int read_counter; 
    int write_counter; 
    int hit_counter; 
    int write_back_counter; 
};

StatisticsInfo *statistics_create(ConfigFile *cfg);
StatisticsInfo *statistics_free(StatisticsInfo *info);
void statistics_add_counter(StatisticsInfo *info, int clock_counter);
void statistics_add_read(StatisticsInfo *info);
void statistics_add_write(StatisticsInfo *info);
void statistics_add_hit_counter(StatisticsInfo *info);
void statistics_add_write_back_counter(StatisticsInfo *info);
void statistics_print(StatisticsInfo *info, FILE *f);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/