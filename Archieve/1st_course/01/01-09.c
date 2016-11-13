#include <stdio.h>

#define swap(a, b) a=a+b, b=a-b, a=a-b

int main(void)
	{
		long n, max1 = -1000000000L, max2 = max1, max3 = max1, i, cur;
		scanf("%ld", &n);
		for (i = 0; i < n; i++)
			{                
				scanf("%ld", &cur);
				if (cur > max3)
					max3 = cur;
				if (max3 > max2)
					swap(max3, max2);
				if (max2 > max1)
					swap(max2, max1);
			}
		printf("%ld %ld %ld\n", max1, max2, max3);
		return 0;
	}