#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

enum
{
    BUF_SIZE = 4096
};

int
update_buffer(char *buf, int *cur_buf_pos)
{
    memset(buf, 0, BUF_SIZE);
    *cur_buf_pos = 0;
    return read(0, buf, BUF_SIZE) > 0;
}

int
is_space(char c)
{
    return c == '\n' || c == '\r' || c == '\t' || c == ' ' || c == 0 || c == EOF;
}

int
get_next_number(char *buf, int *x, int *cur_buf_pos)
{
    int sign = 1, alr_get_num = 0;
    long long res = 0;
    do {
        for (; *cur_buf_pos < BUF_SIZE; (*cur_buf_pos)++) {
            if (buf[*cur_buf_pos] == '-') {
                if (alr_get_num) {
                    *x = res * sign;
                    return 1;
                }
                sign = -1;
                alr_get_num = 1;
                continue;
            }
            if ('0' <= buf[*cur_buf_pos] && buf[*cur_buf_pos] <= '9') {
                res = res * 10 + (buf[*cur_buf_pos] - '0');
                alr_get_num = 1;
                continue;
            }
            if (alr_get_num) {
                (*cur_buf_pos)++;
                *x = res * sign;
                return 1;
            }
        }
    } while (update_buffer(buf, cur_buf_pos));
    *x = res * sign;
    return alr_get_num;
}

int
main(void)
{
    char *buf = (char *)calloc(1, BUF_SIZE);
    int x, cur_buf_pos = 0;
    long long sum = 0;

    while (get_next_number(buf, &x, &cur_buf_pos)) {
        sum += x;
    }

    printf("%lld\n", sum);

    fclose(stdin);
    fclose(stdout);

    return 0;
}
