#include <stdio.h>

int main(void)
	{
		double x, divisor = 1.0, power, res = 0;
		int k, i;
		scanf("%lf%d", &x, &k);
		power = x;
		for (i = 1; i <= k; i++)
			{
				res += ((i%2) ? 1 : -1)*power/divisor;
				power *= x*x;
				divisor *= (2*i)*(2*i + 1);
			}
		printf("%.6lf\n", res);
		return 0;
	}