#include <stdio.h>

int
main(int argc, char *argv[])
{
    int r_time, w_time, wr_time;
    sscanf(argv[1], "%d", &r_time);
    sscanf(argv[2], "%d", &w_time);
    sscanf(argv[3], "%d", &wr_time);

    unsigned long long res = 0;

    char last_type = 0;
    unsigned last_addr = 0;
    int last_size = 0;

    char type = 0, inst_data = 0;
    unsigned addr = 0;
    int size = 0, value = 0;
    while (scanf("%c%c %x %d %d\n", &type, &inst_data, &addr, &size, &value) != EOF) {
        if (last_type == 'W' && type == 'R'
            && last_addr == addr && last_size == size) {
            res -= w_time;
            res += wr_time;
        } else
            res += type == 'W' ? w_time : r_time;

        last_type = type;
        last_addr = addr;
        last_size = size;
    }

    printf("%llu\n", res);

    return 0;
}
