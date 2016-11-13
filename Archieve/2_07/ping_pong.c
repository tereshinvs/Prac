// Ping-pong

#include <stdio.h>

#define N 10

int main(int argc, char *argv[]) {
	int p1[2], p2[2];
	pipe(p1);
	pipe(p2);

	if (!fork()) {
		int buf;
		while (read(p[0], &buf, sizeof(buf)), buf < N) {
			++buf;
			write(p[1], &buf, sizeof(int));
		}
		return 0;
	} else {
		int i = 0;
		write(p[1], &i, sizeof(int));
		while (read(p[0], &i, sizeof(int)), i < N) {
			++i;
			write(p[1], &i, sizeof(i));
			if (i == N)
				break;
		}
		return 0;
	}
}