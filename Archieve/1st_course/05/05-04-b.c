#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 100000

int main(void) {
	int *data = (int *)malloc(BLOCK_SIZE * sizeof(int)), n = 0, alctd = 1, i, res = 0, temp;
	while (1) {
		scanf("%d", &temp);
		if (!temp)
			break;
		if (n == alctd*BLOCK_SIZE)
			data = (int *)realloc(data, ++alctd * BLOCK_SIZE * sizeof(int));
		data[n++] = temp;
	}
	for (i = 0; i < n; i++)
		if (data[i] < data[n - 1])
			res++;
	printf("%d\n", res);
	free(data);
	return 0;
}