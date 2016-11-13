#include <stdio.h>
#include <stdlib.h>

#define next(a) ((a < n - 1) ? a + 1 : 0)
#define prev(a) (a > 0 ? a - 1 : (n - 1))

int
main(int argc, char *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);
    int *buf = (int *)calloc(n, sizeof(int)), pos = 0, temp;
    char *alr = (char *)calloc(n, sizeof(char)), there_is_something = 0;

    while (scanf("%d", &temp) == 1) {
        buf[pos] = temp;
        alr[pos] = 1;
        pos = next(pos);
        there_is_something = 1;
    }

    if (there_is_something) {
        int first = prev(pos);
        while (alr[first]) {
            alr[first] = 0;
            first = prev(first);
        }

        first = next(first);
        do {
            printf("%d\n", buf[first]);
            first = next(first);
        } while (first != pos);
    }    

    free(buf);
    free(alr);
    return 0;
}
