#include <stdio.h>

enum
{
    RECORD_IN_TABLE = 1024,
    PAGE_SIZE = 1024 * 4,
    ADDRESS_OFFSET = 22
};

int
main(int argc, char *argv[])
{
    unsigned start = 0, end = 0;
    char root_table[RECORD_IN_TABLE];
    unsigned long long res = 0;
    for (int i = 0; i < RECORD_IN_TABLE; i++)
        root_table[i] = 0;

    while (scanf("%x-%x", &start, &end) == 2) {
        for (unsigned i = start; i <= end; i += PAGE_SIZE)
            root_table[i >> ADDRESS_OFFSET] = 1;

        while (getchar() != '\n');
    }
    res += PAGE_SIZE;

    for (int i = 0; i < RECORD_IN_TABLE; i++)
        res += PAGE_SIZE * root_table[i];

    printf("%lld\n", res);
    return 0;
}
