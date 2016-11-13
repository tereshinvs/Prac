/* -*- mode:c; coding: utf-8 -*- */

#include "memory.h"

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

static AbstractMemory *
memory_free(AbstractMemory *a)
{
    // FIXME: реализовать
    return NULL;
}

static void
memory_read(AbstractMemory *a, memaddr_t addr, int size, MemoryCell *dst)
{
    Memory *m = (Memory*) a;

    // учитываем время, требуемое на выполнение операции чтения
    statistics_add_counter(m->b.info, (size + m->memory_width - 1) / m->memory_width * m->memory_read_time);
    // выполняем копирование данных
    for (; size; ++addr, --size, ++dst) {
        *dst = m->mem[addr];
    }


static void
memory_write(AbstractMemory *a, memaddr_t addr, int size, const MemoryCell *src)
{
    // FIXME: реализовать
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
    } else if (r < 0 || m->memory_size <= 0 || m->memory_size > MAX_MEM_SIZE || m->memory_size % KiB != 0) {
        error_invalid("memory_create", buf);
    }

    // FIXME: реализовать чтение параметров memory_read_time, memory_write_time, memory_width

    // создаем массив ячеек
    m->mem = (MemoryCell*) calloc(m->memory_size, sizeof(m->mem[0]));

    // возвращяем указатель, преобразованный к указателю на базовую структуру
    return (AbstractMemory*) m;
}

/*
* Local variables:
*  c-basic-offset: 4
* End:
*/