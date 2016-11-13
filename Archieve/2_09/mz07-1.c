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
        pid_t pid2 = fork();
        if (pid2 == 0) {
            dup2(pipefd[1], 1);
            execlp(argv[1], argv[1], NULL);
        } else {
            wait(0);

            pid_t pid4 = fork();
            if (pid4 == 0) {
                int fd = open(argv[4], O_RDONLY);
                dup2(fd, 0);
                dup2(pipefd[1], 1);
                execlp(argv[2], argv[2], NULL);
            } else {
                wait(0);
//                printf("brackets finished\n");
            }
        }
    } else {
        pid_t pid3 = fork();
        if (pid3 == 0) {
            pid_t pid5 = fork();

            if (pid5 == 0) {
                printf("%d\n", getpid());
                dup2(pipefd[0], 0);
                int fd = creat(argv[6], 0666);
                close(fd);
                close(pipefd[0]);
                close(pipefd[1]);
                printf("starting sort\n");
                execlp(argv[3], argv[3], argv[5], NULL);
                return 0;
            } else {
                printf("waiting second part %d\n", pid5);
                wait(0);
                printf("second part finished\n");
            }
        } else {
            close(pipefd[0]);
            close(pipefd[1]);

            if (wait(0) == 
            wait(0);
//            printf("%d\n", wait(0));
//            printf("%d\n", wait(0));
        }
    }

    return 0;
}