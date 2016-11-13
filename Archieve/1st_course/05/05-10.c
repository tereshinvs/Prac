#include <stdio.h>
#include <math.h>

int n;

int is_prime(int t) {
	double up = sqrt(t);
	int i;
	for (i = 2; i <= up; i++)
		if (!(t%i))
			return 0;
	return 1;
}

void find(int cur, int len) {
	int i;
	if (len == n) {
		printf("%d ", cur);
		return;
	}
	for (i = 0; i < 10; i++)
		if (is_prime(cur*10 + i))
			find(cur*10 + i, len + 1);
}

int main(void) {
	scanf("%d", &n);
	find(2, 1); find(3, 1); find(5, 1); find(7, 1);
	printf("\n");
	return 0;
}