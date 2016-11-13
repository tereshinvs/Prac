#include "rand.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int count, low, high, seed;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &low);
    sscanf(argv[3], "%d", &high);
    sscanf(argv[4], "%d", &seed);

    srand(seed);
    for (int i = 0; i < count; i++)
        printf("%d\n", rand_range(low, high));

    return 0;
}
