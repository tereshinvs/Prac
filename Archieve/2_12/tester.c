#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

enum {
	MAX_MSG_VALUE = 1000,
	MAX_MSG_LENGTH = 81
};

typedef struct msgbuf {
	long mtype;
	char mtext[MAX_MSG_LENGTH];
} msgbuf;

void gen_rand_str(char *str, int len) {
	for (int i = 0; i < MAX_MSG_LENGTH; i++)
		str[i] = 0;
	for (int i = 0; i < len; i++)
		str[i] = 'a' + rand() % 26;
}

int main(int argc, char *argv[]) {
	key_t ipc_key = ftok("/usr/bin/shell", '1');
	int msg_ord = msgget(ipc_key, IPC_CREAT | 0777);

	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		int n = rand() % 10;
		for (int j = 0; j < n; j++) {
			msgbuf tmp;
			tmp.mtype = i + 1;
			gen_rand_str(tmp.mtext, 10);

			msgsnd(msg_ord, &tmp, MAX_MSG_LENGTH, IPC_NOWAIT);
		}
	}
}