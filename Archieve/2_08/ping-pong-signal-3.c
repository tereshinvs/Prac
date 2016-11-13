#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int n = 0, brother_pid = -1, id = -1;
FILE *in, *out;

void
son_die(void)
{
    fclose(in);
    fclose(out);
    exit(0);
}

void
sig_handler(int sig)
{
    int tmp;
    if (fscanf(in, "%d", &tmp) != 1)
        son_die();

    if (brother_pid == -1) {
        brother_pid = tmp;
        tmp = 1;
    }

    if (tmp == n)
        son_die();

    printf("%d %d\n", id + 1, tmp);
    fflush(stdout);

    tmp++;
    fprintf(out, "%d\n", tmp);
    fflush(out);
    kill(brother_pid, SIGUSR1);

    if (tmp == n)
        son_die();
}

int
main(int argc, char *argv[])
{
    signal(SIGUSR1, sig_handler);

    sscanf(argv[1], "%d", &n);

    int pipefd[2];
    pipe(pipefd);

    in = fdopen(pipefd[0], "r");
    out = fdopen(pipefd[1], "w");

    id = 0;
    int pid1 = fork();
    if (pid1 == 0) {
        if (n == 1)
            return 0;
        while (1);
        return 0;
    }

    id = 1;
    brother_pid = pid1;
    int pid2 = fork();
    if (pid2 == 0) {
        if (n == 1)
            return 0;
        while (1)
            sleep(1);
        return 0;
    }

    fprintf(out, "%d\n", pid2);
    fflush(out);
    kill(pid1, SIGUSR1);

    fclose(in);
    fclose(out);

    wait(0);
    wait(0);

    printf("Done\n");
    return 0;
}