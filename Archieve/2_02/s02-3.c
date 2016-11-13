#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

enum {
    BUF_SIZE = 50
};

int
get_num(char *s, int *dst)
{
    long long temp = 0, sign = 1;
    if (*s == '-')
        sign = -1, s++;
    for (int i = 0; s[i]; i++) {
        if (s[i] < '0' || s[i] > '9')
            return 0;
        temp = temp * 10 + s[i] - '0';
        if (temp > INT_MAX)
            return 0;
    }
    *dst = (int)temp * sign;
    return 1;
}

unsigned
get_bits(unsigned char src, int st, int len)
{
    return (src / (1 << st)) % (1 << len);
}

int
main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    int first_bit = 0;
    if (!get_num(argv[2], &first_bit) || first_bit < 0) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    int len = 0;
    if (!get_num(argv[3], &len) || len < 1 || len > 32) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    unsigned char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    int first_byte = first_bit / 8;
    int byte_len = (first_bit + len - 1) / 8 - first_byte + 1;
    if (lseek(fd, first_byte, SEEK_SET) == -1) {
        fprintf(stderr, "Input error\n");
        close(fd);
        return 1;
    }
    if (read(fd, buf, byte_len) < byte_len) {
        fprintf(stderr, "Input error\n");
        close(fd);
        return 1;
    }

    if (byte_len == 1) {
        printf("%u\n", get_bits(buf[0], first_bit % 8, len));
        fclose(stdout);
        close(fd);
        return 0;
    }

    int res_len = (first_byte + 1) * 8 - first_bit;
    unsigned res = get_bits(buf[0], first_bit % 8, res_len);

    for (int i = 1; i < byte_len - 1; i++, res_len += 8)
        res += ((unsigned)buf[i]) << res_len;

    res += get_bits(buf[byte_len - 1], 0, len - res_len) << res_len;

    printf("%u\n", res);

    close(fd);
    fclose(stdout);
    return 0;
}
