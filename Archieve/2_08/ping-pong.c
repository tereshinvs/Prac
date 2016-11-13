#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int pipefd1[2];
    int pipefd2[2];
    pipe(pipefd1);
    pipe(pipefd2);

    int n;
    sscanf(argv[1], "%d", &n);

    int pid1 = fork();
    if (!pid1) {
        FILE *in = fdopen(pipefd2[0], "r");
        FILE *out = fdopen(pipefd1[1], "w");

        if (n <= 1)
            return 0;

        while (1) {
            int tmp;

            if (fscanf(in, "%d", &tmp) == EOF)
                break;

            printf("1 %d\n", tmp);
            fflush(stdout);

            if (tmp == n)
                break;

            tmp++;
            if (tmp == n)
                break;

            fprintf(out, "%d ", tmp);
            fflush(out);

            if (tmp == n - 1)
                break;
        }

        fclose(in);
        fclose(out);
    } else {
        int pid2 = fork();
        if (!pid2) {
            FILE *in = fdopen(pipefd1[0], "r");
            FILE *out = fdopen(pipefd2[1], "w");

            if (n <= 1) {
                fflush(out);
                fclose(out);

                fclose(in);
                return 0;
            }

            fprintf(out, "1 ");
            fflush(out);

            if (n == 2)
                return 0;

            while (1) {
                int tmp;

                if (fscanf(in, "%d", &tmp) == EOF)
                    break;

                printf("2 %d\n", tmp);
                fflush(stdout);

                if (tmp == n)
                    break;

                tmp++;
                if (tmp == n)
                    break;

                fprintf(out, "%d ", tmp);
                fflush(out);

                if (tmp == n - 1)
                    break;
            }

            fclose(in);
            fclose(out);
        } else {
            wait(0);
            wait(0);

            printf("Done\n");
            fflush(stdout);
        }
    }
}