#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    BUF_SIZE = 1000
};

int
main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    char *c = calloc(BUF_SIZE + 1, sizeof(char));
    while (fgets(c, BUF_SIZE, in)) {
        int len = strlen(c);
        if (c[len - 1] == '\n') {
            c[len - 1] = '#';
            c[len] = '\n';
            c[len + 1] = 0;
        }
        fprintf(out, "%s", c);
    }
    fclose(in);
    fclose(out);
    return 0;
}
