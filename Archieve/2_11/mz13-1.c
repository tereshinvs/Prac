#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_BUF_SIZE = 100005
};

void
child(int id, int ipc_key, char *str, char *file)
{
    int shmid = shmget(ipc_key, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | 0777);
    if (shmid == -1)
        shmid = shmget(ipc_key, sizeof(int) * 2, 0);

    void *addr = NULL;
    addr = shmat(shmid, addr, 0);

    int sem = semget(ipc_key, 1, IPC_CREAT | IPC_EXCL | 0777);
    if (sem == -1)
        sem = semget(ipc_key, 1, 0);

    FILE *in = fopen(file, "r");

    char *buf = (char *)calloc(MAX_BUF_SIZE, sizeof(char));
    int k = 0;
    while (1) {
        fgets(buf, MAX_BUF_SIZE, in);
        if (strstr(buf, str)) {
            *(int *)addr = id;
            *((int *)addr + 1) = k;
            struct sembuf op;
            op.sem_flg = 0;
            op.sem_num = 0;
            op.sem_op = 1;
            semop(sem, &op, 1);
            break;
        }
        k++;
        if (feof(in))
            break;
    }
    shmdt(addr);
    free(buf);
    fclose(in);
}

int
main(int argc, char *argv[])
{
    int child_num = argc - 3;
    int ipc_key;
//  ipc_key = ftok("/etc/passwd", 'k');
    sscanf(argv[1], "%d", &ipc_key);
    int *pids = (int *)calloc(child_num, sizeof(int));

    int sem = semget(ipc_key, 1, IPC_CREAT | IPC_EXCL | 0777);
    if (sem == -1)
        sem = semget(ipc_key, 1, 0);

    int shmid = shmget(ipc_key, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | 0777);
    if (shmid == -1)
        shmid = shmget(ipc_key, sizeof(int) * 2, 0);
    void *addr = NULL;
    addr = shmat(shmid, addr, 0);

    for (int i = 0; i < child_num; i++)
        if ((pids[i] = fork()) == 0) {
            child(i, ipc_key, argv[2], argv[3 + i]);
            return 0;
        }

    int cnt_pid = fork();
    if (!cnt_pid) {
        struct sembuf op;
        op.sem_flg = 0;
        op.sem_num = 0;
        op.sem_op = -1;
        semop(sem, &op, 1);

        printf("%d %d\n", *(int *)addr + 1, *((int *)addr + 1) + 1);
        shmdt(addr);
        semctl(sem, 1, IPC_RMID);
        return 0;
    }

    for (int i = 0; i < child_num; i++) {
        if (wait(NULL) == cnt_pid) {
            for (int j = i; j < child_num; j++)
                kill(pids[j], SIGTERM);
            free(pids);
            shmdt(addr);
            semctl(sem, 1, IPC_RMID);
            return 0;
        }
    }

    kill(cnt_pid, SIGTERM);

    printf("Not found\n");
    free(pids);
    shmdt(addr);
    semctl(sem, 1, IPC_RMID);
    return 0;
}
