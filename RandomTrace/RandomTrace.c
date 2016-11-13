#include <stdio.h>
#include <stdlib.h>

enum
{
    INIT = 0,
    READ = 1
};

void
print_read(unsigned arr_num)
{
    printf("RD %08x\n", arr_num);
}

void
print_write(unsigned arr_num)
{
    printf("WD %08x\n", arr_num);
}

unsigned
myrand(unsigned low, unsigned high)
{
    return (unsigned)(rand() / (RAND_MAX + 1.0) * (high - low) + low);
}

int
get_random_with_prob(int prob_n, int *probs)
{
    unsigned tmp = myrand(0, 100);
    unsigned pst = 0, sum = 0;
    while (pst < prob_n && sum <= tmp)
        sum += probs[pst++];
    return pst - 1;
}

int
main(int argc, char *argv[])
{
    int cur_state = INIT, p1, p2, p3, seed;
    unsigned low, high, count;

    scanf("%x%x%d%d%d%u%d", &low, &high, &p1, &p2, &p3, &count, &seed);
    srand(seed);

    int probs_init[3], probs_read[2];
    probs_init[0] = p1;
    probs_init[1] = p2;
    probs_init[2] = 100 - p1 - p2;

    probs_read[0] = p3;
    probs_read[1] = 100 - p3;

    unsigned last_read = 0, to_read_addr = 0;

    for (int i = 0; i < count; i++)
        if (cur_state == INIT) {
            int next_move = get_random_with_prob(3, probs_init);
            switch (next_move) {
                case 0:
                    print_write(myrand(low, high));
                    break;
                case 1:
                    print_read(myrand(low, high));
                    break;
                case 2:
                    print_read(to_read_addr = last_read = myrand(low, high));
                    cur_state = READ;
                    break;
                default:
                    break;
            }
        } else {
            int next_move = get_random_with_prob(2, probs_read);
            switch (next_move) {
                case 0:
                    print_read(last_read += 1);
                    break;
                case 1:
                    print_write(to_read_addr);
                    cur_state = INIT;
                    break;
                default:
                    break;
            }
        }

    return 0;
}
