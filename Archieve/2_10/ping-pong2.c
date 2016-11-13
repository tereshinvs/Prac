#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int n;

void
child(int child_num, int *pipefd)
{
    FILE *in = fdopen(pipefd[0], "r");
    FILE *out = fdopen(pipefd[1], "w");

    key_t key = ftok("/etc/passwd", 'a');
    int sem = semget(key, 2, IPC_CREAT | IPC_EXCL | 0777);
    if (sem == -1)
        sem = semget(key, 2, 0);

    struct sembuf sem_op;
    sem_op.sem_flg = 0;

    if (child_num) {
        fprintf(out, "1 ");
        fflush(out);
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        semop(sem, &sem_op, 1);
    }

    while (1) {
        sem_op.sem_num = child_num;
        sem_op.sem_op = -1;
        if (semop(sem, &sem_op, 1) == -1)
            break;
        int tmp;

        if (fscanf(in, "%d", &tmp) != 1)
            break;

        if (tmp == n) {
            semctl(sem, 2, IPC_RMID);
            break;
        }

        printf("%d %d\n", child_num + 1, tmp);
        fflush(stdout);

        fprintf(out, "%d ", tmp + 1);
        fflush(out);
        sem_op.sem_num = 1 - child_num;
        sem_op.sem_op = 1;
        semop(sem, &sem_op, 1);
    }

    fclose(in);
    fclose(out);
}

int
main(int argc, char *argv[])
{
    sscanf(argv[1], "%d", &n);
    int pipefd[2];
    pipe(pipefd);

    if (!fork()) {
        child(0, pipefd);
        return 0;
    }
    if (!fork()) {
        child(1, pipefd);
        return 0;
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(0);
    wait(0);

    printf("Done\n");
    return 0;
}