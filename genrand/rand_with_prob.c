#include <stdio.h>
#include <stdlib.h>

enum
{
    MAX_NUM = 100
};

int
myrand(int low, int high)
{
    return (int)(rand() / (RAND_MAX + 1.0) * (high - low)) + low;
}

int
main(int argc, char *argv[])
{
    int n, m, seed;
    unsigned perc[MAX_NUM][2];

    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%u%u", &perc[i][0], &perc[i][1]);
    scanf("%d%d", &m, &seed);

    srand(seed);

    for (int i = 0; i < m; i++) {
        unsigned tmp = myrand(0, 100);
        unsigned pst = 0, sum = 0;
        while (pst < n && sum <= tmp)
            sum += perc[pst++][1];
        printf("%d\n", perc[pst - 1][0]);
    }
    return 0;
}
