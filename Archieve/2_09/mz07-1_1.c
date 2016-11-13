#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    printf("\"%s\"\n", argv[5]);

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("%d\n", getpid());
        dup2(pipefd[0], 0);
        int fd = creat(argv[6], 0666);
        dup2(fd, 1);
        execlp(argv[3], argv[3], argv[5], NULL);
    } else {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            dup2(pipefd[1], 1);
            execlp(argv[1], argv[1], NULL);
        } else {
            if (wait(0) == pid2) {
                pid_t pid4 = fork();
                if (pid4 == 0) {
                    int fd = open(argv[4], O_RDONLY);
                    dup2(fd, 0);
                    dup2(pipefd[1], 1);
                    execlp(argv[2], argv[2], NULL);
                } else {
                    wait(0);
                }
            } else {
                close(pipefd[0]);
                close(pipefd[1]);
            }
        }
    }

    return 0;
}