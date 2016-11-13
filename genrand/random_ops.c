#include <stdlib.h>
#include <stdio.h>

typedef struct RandomOperations
{
    int seed;
    int alr_got;
    int (*next)(struct RandomOperations *);
} RandomOperations;

int
my_random(RandomOperations *this)
{
    srand(this->seed);

    for (int i = 0; i < this->alr_got; i++)
        rand();

    this->alr_got++;
    return rand();
}

RandomOperations
*random_create(const char *param)
{
    RandomOperations *res = (RandomOperations *)calloc(1, sizeof(RandomOperations));
    sscanf(param, "%d", &res->seed);
    res->alr_got = 0;
    res->next = my_random;

    return res;
}

