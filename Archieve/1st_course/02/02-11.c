#include <stdio.h>

long pow3[20];

long A(long n)
	{
		int i;
		if (n == 1)
			return 0;
		for (i = 0; i < 20; i++)
			if (pow3[i] >= n)
				break;
		i--;
		if (n == pow3[i]*3)
			return A(pow3[i]) + 2*pow3[i];
		if (n == pow3[i]*2)
			return A(1) + pow3[i];
		if (n/(double)pow3[i] < 2)
			return A(pow3[i] - n%pow3[i] + 1) + pow3[i];
		else
			return A(n%pow3[i]) + 2*pow3[i];
	}

int main(void)
	{
		long n, i;
		pow3[0] = 1;
		for (i = 1; i < 20; i++)
			pow3[i] = pow3[i - 1]*3;
		scanf("%ld", &n);
		printf("%ld\n", A(n));
		return 0;
	}