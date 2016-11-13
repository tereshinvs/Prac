#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

struct MyFile
{
    int fd;
    int size;
    char *buf;
    int next_byte;
};

void
myfflush(struct MyFile *f)
{
    for (int i = 0; i < f->next_byte; )
        i += write(f->fd, f->buf + i, f->next_byte - i);
    f->next_byte = 0;
}

void
myputc(int c, struct MyFile *f)
{
    if (f->next_byte == f->size)
        myfflush(f);
    f->buf[f->next_byte++] = (unsigned char)c;
    if (c == '\n')
        myfflush(f);
}
