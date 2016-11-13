#include <stdio.h>
#include <time.h>

int
next_timestamp(FILE *in, time_t *dst)
{
    int year, month, day, hour, minute, second;
    if (fscanf(in, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
        return 0;
    struct tm *tstr;
    time_t rawtime;
    time(&rawtime);
    tstr = localtime(&rawtime);

    tstr->tm_year = year - 1900;
    tstr->tm_mon = month - 1;
    tstr->tm_mday = day;
    tstr->tm_hour = hour;
    tstr->tm_min = minute;
    tstr->tm_sec = second;
    tstr->tm_isdst = -1;

    *dst = mktime(tstr);
    return 1;
}

int
main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");

    time_t last_utc, new_utc = 0;
    next_timestamp(in, &last_utc);

    while (next_timestamp(in, &new_utc)) {
        printf("%lld\n", (long long)new_utc - (long long)last_utc);
        last_utc = new_utc;
    }

    fclose(in);
    return 0;
}
