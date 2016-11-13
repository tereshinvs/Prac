	// pr1 > a1 && pr2 < a2

#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	int pid = fork();

	if (pid == 0) {
		int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777); // 0777 - потому что процесс эндит это число со своим umask
		dup2(fd, 1);
		execvp(argv[1], argv[1], NULL);
	} else {
		int w_status = 0;
		wait(&w_status);
		if (WIFEEXITED(w_status)) {
			return 1;
		}
		w_status = WEXITSTATUS(w_status);
	}

	pid = fork();
	if (pid == 0) {
		int fd = open(argv[4], O_RDONLY);
		dup2(fd, 0);
		execvp(argv[3], argv[3], NULL);
	} else {
		int w_status = 0;
		wait(&w_status);
		if (WIFEEXITED(w_status)) {
			return 1;
		}
		w_status = WEXITSTATUS(w_status);
	}
	return 0;
}