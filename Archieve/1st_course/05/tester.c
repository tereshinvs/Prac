#include <stdio.h>

int main(void) {
	int i, j, n = 1000;
	printf("%d %d 0 0 %d %d\n", n, n, n - 1, n - 1);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("%d ", 10000*((i + j)%2));
		printf("\n");
	}
	return 0;
}