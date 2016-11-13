#include <stdio.h>

int main(void)
	{
		unsigned long n;
		scanf("%lu", &n);
		printf("%lu", n%(1 << 24) + (~(n/(1 << 24))) << 24);
		return 0;
	}