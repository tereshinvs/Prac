#include <stdio.h>

#define swap(a, b) a=a+b,b=a-b,a=a-b

int main(void)
	{
		int n, cur, i;
		long long min1 = 10000000000LL, min2 = min1, max1 = -min1, max2 = max1;
		scanf("%d", &n);
		for (i = 0; i < n; i++)
			{
				scanf("%d", &cur);
				if (cur < min2) min2 = cur;
				if (min1 > min2) swap(min1, min2);
				if (cur > max2) max2 = cur;
				if (max1 < max2) swap(max1, max2);
			}
		if (max1*max2 > min1*min2)
			printf("%lld %lld\n", max2, max1);
		else
			printf("%lld %lld\n", min1, min2);
		return 0;
	}