#include <stdio.h>

int
main(int argc, char *argv[])
{
    unsigned start = 0, end = 0;
    long long res = 0;

    while (scanf("%x-%x", &start, &end) == 2) {
        res += end - start;
        while (getchar() != '\n');
    }

    printf("%lld\n", res);
    return 0;
}
