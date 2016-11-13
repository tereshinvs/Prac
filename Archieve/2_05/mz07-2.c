#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[]) {
	int pipefd1[2];
	pipe(pipefd1);

	int pid = fork();
	if (!pid) {
		close(pipefd1[0]);
		dup2(pipefd1[1], 1);
		execlp(argv[1], argv[1], NULL);
	} else {
		int pipefd2[2];
		pipe(pipefd2);

		pid = fork();
		if (!pid) {
			close(pipefd1[1]);
			dup2(pipefd1[0], 0);
			close(pipefd2[0]);
			dup2(pipefd2[1], 1);
			execlp(argv[2], argv[2], NULL);
		} else {
			wait(0);

			pid = fork();
			if (!pid) {
				close(pipefd1[1]);
				dup2(pipefd1[0], 0);
				close(pipefd2[0]);
				dup2(pipefd2[1], 1);
				execlp(argv[3], argv[3], NULL);
			} else {
				pid = fork();
				if (!pid) {
					close(pipefd1[0]);
					close(pipefd1[1]);
					close(pipefd2[1]);
					dup2(pipefd2[0], 0);
					execlp(argv[4], argv[4], NULL);
				} else {
					wait(0);
					close(pipefd1[0]);
					close(pipefd1[1]);
					close(pipefd2[0]);
					close(pipefd2[1]);
				}
			}
		}
	}
	return 0;
}