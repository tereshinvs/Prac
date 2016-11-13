#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int
main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);
    if (!fork()) {
        dup2(pipefd[0], 0);
        close(pipefd[1]);

        if (!fork()) {
            if (!fork()) {
                time_t read_time;
                read(0, &read_time, sizeof(read_time));
                struct tm *_tm = localtime(&read_time);
                printf("S:%.2d\n", _tm->tm_sec);
            } else {
                wait(0);
                time_t read_time;
                read(0, &read_time, sizeof(read_time));
                struct tm *_tm = localtime(&read_time);
                printf("M:%.2d\n", _tm->tm_min);
            }
        } else {
            wait(0);
            time_t read_time;
            read(0, &read_time, sizeof(read_time));
            struct tm *_tm = localtime(&read_time);
            printf("H:%.2d\n", _tm->tm_hour);
        }
    } else {
        time_t curtime;
        time(&curtime);

        write(pipefd[1], &curtime, sizeof(time_t));
        write(pipefd[1], &curtime, sizeof(time_t));
        write(pipefd[1], &curtime, sizeof(time_t));

        wait(0);
    }

    return 0;
}