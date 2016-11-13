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

int strbuf_cmp(const void *_t1, const void *_t2) {
	msgbuf *t1 = (msgbuf *)_t1;
	msgbuf *t2 = (msgbuf *)_t2;
	return strcmp(t1->mtext, t2->mtext);
}

int receive_all_msg(int msg_ord, int ord_num, msgbuf *dst) {
	int k = 0;
	while (1)
		if (msgrcv(msg_ord, &dst[k++], MAX_MSG_LENGTH, ord_num, IPC_NOWAIT) == -1)
			break;
	return k - 1;
}

void sort_msg_order(int msg_ord, int ord_num) {
	msgbuf *data = (msgbuf *)calloc(MAX_MSG_VALUE, sizeof(msgbuf));

	int k = receive_all_msg(msg_ord, ord_num, data);
	printf("K: %d %d\n", ord_num, k);
	printf("%d received\n", ord_num);
	qsort(data, k, sizeof(msgbuf), strbuf_cmp);

	for (int i = 0; i < k; i++) {
		printf("!!! %ld\n", data[i].mtype);
		msgsnd(msg_ord, &data[i], MAX_MSG_LENGTH, IPC_NOWAIT);
	}
	printf("%d sended\n", ord_num);

	free(data);
}

void child(int id, int left, int right, int msg_ord) {
	printf("child #%d\n", id);
	for (int i = left; i < right; i++)
		sort_msg_order(msg_ord, i + 1);
}

int main(int argc, char *argv[]) {
	key_t ipc_key = ftok("/usr/bin/shell", '1');
	int msg_ord = msgget(ipc_key, 0);

	for (int i = 0; i < 5; i++)
		if (!fork()) {
			child(i, i * 2, i * 2 + 2, msg_ord);
			return 0;
		}

	printf("waiting...\n");
	for (int i = 0; i < 5; i++)
		wait(NULL);
	printf("end waiting...\n");

	for (int i = 0; i < 10; i++) {
		msgbuf *data = (msgbuf *)calloc(MAX_MSG_VALUE, sizeof(msgbuf));
		int k = receive_all_msg(msg_ord, i, data);

		for (int j = k - 1; j >= 0; j++)
			msgsnd(msg_ord, &data[j], MAX_MSG_LENGTH, IPC_NOWAIT);

		free(data);
	}

	return 0;
}