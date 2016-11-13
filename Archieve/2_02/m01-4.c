#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

struct node
{
    int key;
    int left_idx;
    int right_idx;
};

int fd;

void
read_node(int pos, struct node *dst)
{
    lseek(fd, pos * sizeof(struct node), SEEK_SET);
    for (int alr_read = 0; alr_read < sizeof(struct node);) {
        int temp = read(fd, (char *)dst + alr_read, sizeof(struct node) - alr_read);
        if (temp != -1)
            alr_read += temp;
    }
}

void
read_and_print(int pos)
{
    struct node a;
    read_node(pos, &a);
    if (a.right_idx) {
        read_and_print(a.right_idx);
    }
    printf("%d ", a.key);
    if (a.left_idx)
        read_and_print(a.left_idx);
}

int
main(int argc, char *argv[])
{
    fd = open(argv[1], O_RDONLY);
    read_and_print(0);
    close(fd);
    fclose(stdout);
    return 0;
}
