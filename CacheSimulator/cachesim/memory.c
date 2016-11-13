/* -*- mode:c; coding: utf-8 -*- */

#include "memory.h"
#include "parse_config.h"

#include <stdlib.h>

enum
{
    MAX_MEM_SIZE = 1 * GiB, 
    MAX_READ_TIME = 100000, 
    MAX_WRITE_TIME = MAX_READ_TIME, 
    MAX_WIDTH = 1024 
};

struct Memory;
typedef struct Memory Memory;

struct Memory
{
    AbstractMemory b; 
    MemoryCell *mem; 
    int memory_size; 
    int memory_read_time; 
    int memory_write_time; 
    int memory_width; 
};

AbstractMemory *
memory_free(AbstractMemory *a)
{
    Memory *b = (Memory *)a;
    free(b->mem);
    free(b);
    return NULL;
}

static void
memory_read(AbstractMemory *a, memaddr_t addr, int size, MemoryCell *dst)
{
    Memory *m = (Memory*) a;

    // учитываем время, требуемое на выполнение операции чтения
    statistics_add_counter(m->b.info, (size + m->memory_width - 1) / m->memory_width * m->memory_read_time);
    statistics_add_read(m->b.info);
    // выполняем копирование данных
    for (; size; ++addr, --size, ++dst) {
//        *dst = m->mem[addr];
        if (addr >= m->memory_size)
            break;
        m->mem[addr] = *dst;
        m->mem[addr].flags = 1;
    }
}


static void
memory_write(AbstractMemory *a, memaddr_t addr, int size, const MemoryCell *src)
{
    Memory *m = (Memory *)a;

    statistics_add_counter(m->b.info, (size + m->memory_width - 1) / m->memory_width * m->memory_write_time);
    statistics_add_write(m->b.info);

    for (; size; ++addr, --size, ++src) {
        if (addr >= m->memory_size)
            break;
        m->mem[addr] = *src;
        m->mem[addr].flags = 1;
    }
}

static void
memory_reveal(AbstractMemory *a, memaddr_t addr, int size, const MemoryCell *src)
{
    Memory *m = (Memory*) a;

    for (; size; ++addr, --size, ++src) {
        m->mem[addr] = *src;
    }
}

static void
memory_flush(AbstractMemory *m)
{
}

static AbstractMemoryOps memory_ops =
{
    memory_free,
    memory_read,
    memory_write,
    memory_reveal,
    memory_flush,
};

AbstractMemory *
memory_create(ConfigFile *cfg, const char *var_prefix, StatisticsInfo *info)
{
    char buf[1024];
    Memory *m = calloc(1, sizeof(*m));
    int r;

    // заполняем базовые поля
    m->b.ops = &memory_ops;
    m->b.info = info;

    // считываем и проверяем параметр memory_size
    r = config_file_get_int(cfg, make_param_name(buf, sizeof(buf), var_prefix, "memory_size"), &m->memory_size);
    if (!r) {
        error_undefined("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;
    } else if (r < 0 || m->memory_size <= 0 || m->memory_size > MAX_MEM_SIZE || m->memory_size % KiB != 0) {
        error_invalid("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;
    }

    r = config_file_get_int(cfg, make_param_name(buf, sizeof(buf), var_prefix, "memory_read_time"), &m->memory_read_time);
    if (!r) {
        error_undefined("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;
    } else if (r < 0 || m->memory_read_time <= 0 || m->memory_read_time > MAX_READ_TIME) {
        error_invalid("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;        
    }
    r = config_file_get_int(cfg, make_param_name(buf, sizeof(buf), var_prefix, "memory_write_time"), &m->memory_write_time);
    if (!r) {
        error_undefined("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;
    } else if (r < 0 || m->memory_write_time <= 0 || m->memory_write_time > MAX_WRITE_TIME) {
        error_invalid("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;        
    }
    r = config_file_get_int(cfg, make_param_name(buf, sizeof(buf), var_prefix, "memory_width"), &m->memory_width);
    if (!r) {
        error_undefined("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;
    } else if (r < 0 || m->memory_width <= 0 || m->memory_width > MAX_WIDTH) {
        error_invalid("memory_create", buf);
        memory_free((AbstractMemory *)m);
        return NULL;        
    }

    // создаем массив ячеек
    m->mem = (MemoryCell*) calloc(m->memory_size, sizeof(m->mem[0]));

    // возвращяем указатель, преобразованный к указателю на базовую структуру
    return (AbstractMemory*) m;
}

int
memory_get_cell(AbstractMemory *mem, int num, int *dst)
{
    Memory *memory = (Memory *)mem;
    *dst = memory->mem[num].value;
    return memory->mem[num].flags;
}

int
memory_get_size(AbstractMemory *mem)
{
    return ((Memory *)mem)->memory_size;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/