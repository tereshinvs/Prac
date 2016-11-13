#include <stdio.h>

int main(void)
	{
		unsigned long n, k;
		scanf("%lu%lu", &n, &k);
		printf("%lu\n", n%(1 << k));
		return 0;
	}