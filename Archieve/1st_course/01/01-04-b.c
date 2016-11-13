#include <stdio.h>

int main(void)
	{
		unsigned long n, k, i, maximum = 0, temp;
		scanf("%lu%lu", &n, &k);
		for (i = k - 1; i < sizeof(long)*8; i++)
			{
				temp = n%(1 << (i + 1))/(1 << (sizeof(long)*8 - i - 1));
				if (temp > maximum)
					maximum = temp;
			}
		printf("%d\n", maximum);
		return 0;
	}