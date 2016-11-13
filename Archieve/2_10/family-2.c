#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int mod, n;
    sscanf(argv[2], "%d", &mod);
    sscanf(argv[3], "%d", &n);

    int pipefd[2];
    pipe(pipefd);

    int pid1 = fork();
    if (pid1 == 0) {
        close(pipefd[1]);
        int tmp = 0, sum = 0, nephew = 0;
        read(pipefd[0], &nephew, sizeof(int));
        while (read(pipefd[0], &tmp, sizeof(int)) == sizeof(int)) {
            sum += tmp;
            if (sum > n) {
                printf("-1\n");
                close(pipefd[1]);
                kill(nephew, SIGTERM);
                return 0;
            }
        }
        close(pipefd[0]);
        printf("%d\n", sum);
        return 0;
    }

    int pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[0]);
        int pid3 = fork();

        if (pid3 == 0) {
            int mypid = getpid();
            write(pipefd[1], &mypid, sizeof(int));
            FILE *in = fopen(argv[1], "r");
            int tmp = 0;
            while (fscanf(in, "%d", &tmp) != EOF) {
                tmp = tmp * tmp % mod;
                write(pipefd[1], &tmp, sizeof(int));
            }
            fflush(stdout);
            close(pipefd[1]);
            return 0;
        }

        close(pipefd[1]);
        wait(0);
        return 0;
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(0);
    wait(0);

    printf("0\n");
    return 0;
}