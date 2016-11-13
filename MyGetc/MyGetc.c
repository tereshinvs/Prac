#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

struct MyFile
{
    int fd;
    int size;
    char *buf;
    int byte_in_buf;
    int next_byte;
};

void update_buf(struct MyFile *f)
{
    f->byte_in_buf = read(f->fd, f->buf, f->size);
    f->next_byte = 0;
}

int mygetc(struct MyFile *f)
{
    if (f->next_byte == f->byte_in_buf)
        update_buf(f);
    if (f->byte_in_buf == 0)
        return EOF;
    return (unsigned char)f->buf[f->next_byte++];
}

