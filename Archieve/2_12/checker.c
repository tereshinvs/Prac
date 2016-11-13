#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

enum {
	MAX_MSG_VALUE = 1000,
	MAX_MSG_LENGTH = 81
};

typedef struct msgbuf {
	long mtype;
	char mtext[MAX_MSG_LENGTH];
} msgbuf;

int main(int argc, char *argv[]) {
	key_t ipc_key = ftok("/usr/bin/shell", '1');
	int msg_ord = msgget(ipc_key, IPC_CREAT | 0777);

	while (1) {
		msgbuf tmp;
		if (msgrcv(msg_ord, &tmp, MAX_MSG_VALUE, 0, IPC_NOWAIT) == -1)
			break;

//		if (errno != MSG_NOERROR)
//			break;

		printf("%ld %s\n", tmp.mtype, tmp.mtext);
	}

	return 0;
}