#include <stdio.h>

int main(void)
	{
		long n, res = 0;
		scanf("%ld", &n);
		for (; n >= 3; n /= 3)
			switch (n%3)
				{
					case 1: res += 1; break;
					case 2: res += 2; break;
					default: break;
				}
		printf("%ld\n", res);
		return 0;
	}