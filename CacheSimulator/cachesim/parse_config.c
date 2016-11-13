/* -*- mode:c; coding: utf-8 -*- */

#include "parse_config.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

enum
{
    LINE_BUF_SIZE = 1024,
    MAX_LINE_LENGTH = 1000
};

typedef struct ConfigParam
{
    char *name; 
    char *value; 
} ConfigParam;

struct ConfigFile
{
    int size; 
    int reserved; 
    ConfigParam *params; 
};

char *
strip(char *s)
{
    while (*s == ' ')
        s++;

    char *end = s + strlen(s);
    while (*end == ' ' || *end == '\n' || *end == 0 || *end == '\r' || *end == '\t') {
        *end = 0;
        end--;
    }

    return s;
}

int
is_empty(const char *s)
{
    for (const char *c = s; *c != 0 && *c != '\n'; c++)
        if (*c != ' ')
            return 0;
    return 1;
}

int
is_correct_name(const char *s)
{
    if (!isalpha(s[0]) && s[0] != '_')
        return 0;

    for (int i = 0; s[i]; i++)
        if (!isalpha(s[i]) && !isdigit(s[i]) && s[i] != '_' && s[i] != '-')
            return 0;

    return 1;
}

ConfigFile *
config_file_parse(const char *path, FILE *log_f)
{
    ConfigFile *cfg = (ConfigFile*) calloc(1, sizeof(*cfg));

    // FIXME: реализовать разбор конфигурационного файла и сортировку массива params
    FILE *conf_in = fopen(path, "r");
    if (!conf_in) {
        fprintf(log_f, "Failed to open %s for reading\n", path);
        fflush(log_f);
        return NULL;
    }

    cfg->params = (ConfigParam *)calloc(sizeof(ConfigParam), 0);
    cfg->size = cfg->reserved = 0;

    char *buf = (char *)calloc(sizeof(char), LINE_BUF_SIZE);
    int cur_line = 0;

    while (fgets(buf, MAX_LINE_LENGTH, conf_in)) {

        char *line_break = strchr(buf, '\n');
        if (line_break)
            *line_break = 0;

        cur_line++;
        char *com_st = strchr(buf, '#');
        if (com_st)
            *com_st = 0;

        char *eq_smb = strchr(buf, '=');
        if (!eq_smb) {
            if (!is_empty(buf)) {
                fprintf(log_f, "Syntax error in line %d of %s\n", cur_line, path);
                fflush(log_f);
                config_file_free(cfg);
                fclose(conf_in);
                free(buf);
                return NULL;
            }
            continue;
        }
        *eq_smb = 0;

        char *name_st = buf, *value_st = eq_smb + 1;

        name_st = strip(name_st);
        value_st = strip(value_st);

        if (!is_correct_name(name_st)) {
            fprintf(log_f, "Syntax error in line %d of %s\n", cur_line, path);
            fflush(log_f);
            config_file_free(cfg);
            fclose(conf_in);
            free(buf);
            return NULL;
        }

        cfg->size++;
        cfg->reserved++;
        cfg->params = (ConfigParam *)realloc(cfg->params, sizeof(ConfigParam) * cfg->size);
        cfg->params[cfg->size - 1].name = NULL;
        cfg->params[cfg->size - 1].value = NULL;

        int pst = cfg->size - 1;
        for (pst = cfg->size - 2; pst >= 0; pst--) {
            int tmp = strcmp(name_st, cfg->params[pst].name);
            if (tmp == 0) {
                cfg->params[pst + 1].name = cfg->params[pst + 1].value = NULL;
                config_file_free(cfg);
                fclose(conf_in);
                fprintf(log_f, "Duplicate parameter %s in %s\n", name_st, path);
                fflush(log_f);
                free(buf);
                return NULL;
            }
            if (tmp > 0)
                break;

            cfg->params[pst + 1].name = cfg->params[pst].name;
            cfg->params[pst + 1].value = cfg->params[pst].value;
        }
        pst++;

        cfg->params[pst].name = (char *)calloc(sizeof(char), strlen(name_st) + 1);
        strcpy(cfg->params[pst].name, name_st);

        cfg->params[pst].value = (char *)calloc(sizeof(char), strlen(value_st) + 1);
        strcpy(cfg->params[pst].value, value_st);
    }

    free(buf);
    fclose(conf_in);

    return cfg;
}

ConfigFile *
config_file_free(ConfigFile *cfg)
{
    if (!cfg)
        return NULL;

    for (int i = 0; i < cfg->size; i++) {
        if (cfg->params[i].name)
            free(cfg->params[i].name);
        if (cfg->params[i].value)
            free(cfg->params[i].value);
    }
    if (cfg->params)
        free(cfg->params);
    if (cfg)
        free(cfg);

    return NULL;
}

const char *
config_file_get(const ConfigFile *cfg, const char *name)
{
    int left = 0, right = cfg->size - 1;
    while (left <= right) {
        int med = (left + right) / 2;
        int tmp = strcmp(name, cfg->params[med].name);
        if (tmp == 0)
            return cfg->params[med].value;
        if (tmp > 0)
            left = med + 1;
        else
            right = med - 1;
    }

    return NULL;
}

int
config_file_get_int(const ConfigFile *cfg, const char *name, int *p_value)
{
    const char *str = config_file_get(cfg, name);
    if (!str) {
        return 0;
    }
    char *eptr = NULL;
    errno = 0;
    int value = strtol(str, &eptr, 10);
    if (*eptr || errno) {
        return -1;
    }
    *p_value = value;
    return 1;
}

const char *
config_file_get_nth_name(const ConfigFile *cfg, int n)
{
    return n < cfg->size ? cfg->params[n].name : NULL;
}

int
config_file_length(const ConfigFile *cfg)
{
    return cfg->size;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/