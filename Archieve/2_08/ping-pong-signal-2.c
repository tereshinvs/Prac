#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

enum {
	FIRST_SON_GID = 1234,
	SECOND_SON_GID = 6543
};

int cur_proc, flag = 0, n;

void sig_handler(int sig) {
	printf("%d\n", cur_proc);
	fflush(stdout);
	flag = 1;
}

void child(int child_num, int *pipefd) {
	signal(SIGUSR1, sig_handler);
	cur_proc = child_num;
//	printf("%d\n", setpgid(0, child_num ? SECOND_SON_GID : FIRST_SON_GID));
//	fflush(stdout);

	FILE *in = fdopen(pipefd[0], "r");
	FILE *out = fdopen(pipefd[1], "w");

	if (child_num) {
		fprintf(out, "1 ");
		fflush(out);
//		kill(FIRST_SON_GID, SIGUSR1);
		kill(0, SIGUSR1);
	}

	printf("%d start\n", child_num);
	fflush(stdout);
	while (1) {
		while (!flag) {
//			printf("%d\n", child_num);
//			fflush(stdout);
		}

		int tmp;
//		printf("%d reading\n", child_num);
//		fflush(stdout);
		if (fscanf(in, "%d", &tmp) == EOF)
			break;
//		printf("%d reading end\n", child_num);
//		fflush(stdout);

		if (tmp == n) {
//			kill(child_num ? -FIRST_SON_GID : -SECOND_SON_GID, SIGTERM);
			kill(0, SIGTERM);
			break;
		}

		printf("%d %d\n", child_num + 1, tmp);
		fflush(stdout);

		fprintf(out, "%d ", tmp + 1);
		fflush(stdout);
//		kill(child_num ? -FIRST_SON_GID : -SECOND_SON_GID, SIGUSR1);
		kill(0, SIGUSR1);

		flag = 0;
	}

	fclose(in);
	fclose(out);
}

int main(int argc, char *argv[]) {
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