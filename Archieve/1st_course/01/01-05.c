#include <stdio.h>

int main(void)
	{
		unsigned long n, i, res = 0;
		scanf("%lu", &n);
		for (i = 0; i < sizeof(long)*8; i++, n >>= 1)
			res += n%2;
		printf("%lu\n", res);
		return 0;
	}