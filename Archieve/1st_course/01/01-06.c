#include <stdio.h>

int main(void)
	{
		unsigned long a, b, c, d;
		scanf("%lu%lu%lu%lu", &a, &b, &c, &d);
		printf("%lu\n", (d << 24) + (c << 16) + (b << 8) + a);
		return 0;
	}