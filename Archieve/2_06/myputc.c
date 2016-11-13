#include <unistd.h>

struct MyFile
{
    int fd;
    int size;
    int next_byte;
    char *buf;
};

void out_buf(struct MyFile *f) {
    write(f->fd, f->buf, f->next_byte);
    f->next_byte = 0;
}

void myputc(int c, struct MyFile *f) {
    if (f->next_byte == f->size)
        out_buf(f);
    f->buf[f->next_byte++] = c;
    if (c == '\n')
        out_buf(f);
}

/*int main(int argc, char *argv[]) {
    return 0;
}*/
