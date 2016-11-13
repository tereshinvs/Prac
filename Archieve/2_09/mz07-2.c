#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int pipefd1[2];
    pipe(pipefd1);

    int pipefd2[2];
    pipe(pipefd2);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd1[1], 1);
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execlp(argv[1], argv[1], NULL);
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd1[0], 0);
        dup2(pipefd2[1], 1);
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        pid_t pid = fork();
        if (pid == 0) {
            execlp(argv[2], argv[2], NULL);
            return 1;
        }

        wait(0);
        pid = fork();
        if (pid == 0) {
            execlp(argv[3], argv[3], NULL);
            return 1;
        }

        wait(0);
        return 0;
    }

    close(pipefd1[0]);
    close(pipefd1[1]);

    pid_t pid3 = fork();
    if (pid3 == 0) {
        dup2(pipefd2[0], 0);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execlp(argv[4], argv[4], NULL);
        return 1;
    }

    close(pipefd2[0]);
    close(pipefd2[1]);

    wait(0);
    wait(0);
    wait(0);
    return 0;
}