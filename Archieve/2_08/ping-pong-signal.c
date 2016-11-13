#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int pipefd[2];
int pid1, pid2;
int n;

int flag1 = 0, flag2 = 0;

FILE *in = NULL, *out = NULL;

void
parent_handler_usr1(int sig)
{
    signal(SIGUSR1, parent_handler_usr1);
    kill(pid1, SIGUSR1);
}

void
parent_handler_usr2(int sig)
{
    signal(SIGUSR2, parent_handler_usr2);
    kill(pid2, SIGUSR1);
}

void
proc1_handler(int sig)
{
    signal(SIGUSR1, proc1_handler);
    flag1++;
}

void
proc2_handler(int sig)
{
    signal(SIGUSR1, proc2_handler);
    flag2++;
}

int
main(int argc, char *argv[])
{
    signal(SIGUSR1, parent_handler_usr1);
    signal(SIGUSR2, parent_handler_usr2);

    sscanf(argv[1], "%d", &n);
    pipe(pipefd);

    pid1 = fork();
    if (!pid1) {
        signal(SIGUSR1, proc1_handler);
        if (n <= 1)
            exit(0);

        in = fdopen(pipefd[0], "r");
        out = fdopen(pipefd[1], "w");

        while (1) {
            while (flag1 == 0);

            flag1--;
            int tmp;
            if (fscanf(in, "%d", &tmp) == EOF) {
                fclose(in);
                fclose(out);
                close(pipefd[0]);
                close(pipefd[1]);
                exit(0);                
            }

            if (tmp == n) {
                fclose(in);
                fclose(out);
                close(pipefd[0]);
                close(pipefd[1]);
                exit(0);
            }

            printf("1 %d\n", tmp);
            fflush(stdout);

            tmp++;
            fprintf(out, "%d ", tmp);
            fflush(out);
            kill(getppid(), SIGUSR2);           
        }
    } else {
        pid2 = fork();
        if (!pid2) {
            signal(SIGUSR1, proc2_handler);
            if (n <= 1)
                exit(0);
            in = fdopen(pipefd[0], "r");
            out = fdopen(pipefd[1], "w");
            fprintf(out, "%d ", 1);
            fflush(out);
            kill(getppid(), SIGUSR1);
            if (n == 2)
                exit(0);
            while (1) {
                while (flag2 == 0);
                flag2--;

                int tmp;
                if (fscanf(in, "%d", &tmp) == EOF) {
                    fclose(in);
                    fclose(out);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    exit(0);                    
                }

                if (tmp == n) {
                    fclose(in);
                    fclose(out);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    exit(0);
                }

                printf("2 %d\n", tmp);
                fflush(stdout);

                tmp++;
                fprintf(out, "%d ", tmp);
                fflush(out);
                kill(getppid(), SIGUSR1);
            }
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            wait(0);
            wait(0);
            printf("Done\n");
        }
    }
    return 0;
}