/* -*- mode:c; coding: utf-8 -*- */

#include "statistics.h"

#include <stdlib.h>

StatisticsInfo *
statistics_create(ConfigFile *cfg)
{
	return (StatisticsInfo *)calloc(1, sizeof(StatisticsInfo));
}

void
statistics_add_counter(StatisticsInfo *info, int clock_counter)
{
    info->clock_counter += clock_counter;  // FIXED ???
}

void
statistics_add_hit_counter(StatisticsInfo *info)
{
    ++info->hit_counter;
}

void
statistics_add_write_back_counter(StatisticsInfo *info)
{
    ++info->write_back_counter;
}

void
statistics_add_read(StatisticsInfo *info)
{
	++info->read_counter;
}

void
statistics_add_write(StatisticsInfo *info)
{
	++info->write_counter;
}

StatisticsInfo *
statistics_free(StatisticsInfo *info)
{
	free(info);
	return NULL;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/