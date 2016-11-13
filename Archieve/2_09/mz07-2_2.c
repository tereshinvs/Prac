#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int pipefd1[2];
    pipe(pipefd1);

    int pipefd2[2];
    pipe(pipefd2)

    pid_t pid1;
    if (!(pid1 = fork())) {
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp(argv[1], argv[1], NULL);        
        return 1;
    }

    if (!fork()) {
        int fd = creat(argv[6], 0666);
        dup2(fd, 1);
        close(fd);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp(argv[3], argv[3], argv[5], NULL);        
        return 1;
    }

    close(pipefd[0]);
    if (pid1 == wait(0)) {
        if (!fork()) {
            int fd = open(argv[4], O_RDONLY);
            dup2(fd, 0);
            close(fd);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            execlp(argv[2], argv[2], NULL);
            return 1;
        }
        wait(0);
    } else {
        wait(0);
        if (!fork()) {
            int fd = open(argv[4], O_RDONLY);
            dup2(fd, 0);
            close(fd);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            execlp(argv[2], argv[2], NULL);
            return 1;
        }
    }

    close(pipefd[1]);
    wait(0);
    return 0;
}
