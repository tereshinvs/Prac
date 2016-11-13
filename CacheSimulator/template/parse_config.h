/* -*- mode:c; coding: utf-8 -*- */

#ifndef PARSE_CONFIG_H_INCLUDED
#define PARSE_CONFIG_H_INCLUDED

#include <stdio.h>

struct ConfigFile;
typedef struct ConfigFile ConfigFile;

ConfigFile *config_file_parse(const char *path, FILE *log_f);

ConfigFile *config_file_free(ConfigFile *cfg);

const char *config_file_get(const ConfigFile *cfg, const char *name);

int config_file_get_int(const ConfigFile *cfg, const char *name, int *p_value);

#endif

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/