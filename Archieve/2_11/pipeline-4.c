#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void
execute_cmd(int cur, int *last_pipe, int argc, char *argv[])
{
    int pipefd[2];
    if (cur < argc - 1)
        pipe(pipefd);

    if (!fork()) {
        if (cur > 1) {
            dup2(last_pipe[0], 0);
            close(last_pipe[0]);
            close(last_pipe[1]);
        }
        if (cur < argc - 1) {
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            close(pipefd[0]);
        }
        execlp(argv[cur], argv[cur], NULL);
    } else {
        if (last_pipe) {
            close(last_pipe[0]); 
            close(last_pipe[1]);
        }
        if (cur < argc - 1) {
            execute_cmd(cur + 1, pipefd, argc, argv);
            close(pipefd[0]);
            close(pipefd[1]);           
        } else {
            close(pipefd[0]);
            close(pipefd[1]);           
        }
        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    execute_cmd(1, NULL, argc, argv);
    return 0;
}