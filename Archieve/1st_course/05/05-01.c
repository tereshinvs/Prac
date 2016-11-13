#include <stdio.h>
#include <stdlib.h>

struct complex {
	long long real, imag;
};

int main(void) {
	int n, i, j;
	struct complex *data;
	scanf("%d", &n);
	data = (struct complex *)malloc(n * n * sizeof(struct complex));
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			scanf("%lld%lld", &(data + i*n + j)->real, &(data + i*n + j)->imag);
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++)
			printf("%lld %lld ", (data + i*n + j)->real, -(data + i*n + j)->imag);
		printf("\n");
	}
	free(data);
	return 0;
}