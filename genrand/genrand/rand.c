#include "rand.h"
#include <stdlib.h>

int rand_range(int low, int high) {
    return (int)(rand() / (RAND_MAX + 1.0) * (high - low)) + low;
}
