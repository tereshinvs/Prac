#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if (!pid) {
        close(pipefd[0]);
        dup2(pipefd[1], 1);

        int pid1 = fork();
        if (!pid1)
            execlp(argv[1], argv[1], NULL);
        else {
            wait(0);
            int fd = open(argv[4], O_RDONLY);
            dup2(fd, 0);
            execlp(argv[2], argv[2], NULL);
        }
    } else {
        int fd = open(argv[6], O_WRONLY | O_CREAT | O_TRUNC, 0777);

        int pid1 = fork();
        if (!pid1) {
            dup2(fd, 1);
            dup2(pipefd[0], 0);
            close(pipefd[1]);
            execlp(argv[3], argv[3], argv[5], NULL);
        } else {
            waitpid(-1, 0, WNOHANG);
            close(fd);
            close(pipefd[0]);
            close(pipefd[1]);
        }
    }

    return 0;
}