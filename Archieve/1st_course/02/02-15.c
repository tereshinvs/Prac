#include <stdio.h>

unsigned f(unsigned x)
	{
   	long long s = 0;
   	unsigned i;
    	for (i = 0; s <= x; i++)
    		s += 2 * i + 1;
	   return x * (i - 1);
	}

unsigned g(unsigned t)
	{
		unsigned left = 0, right = 4294967295U, med, fmed;
		while (left <= right)
			{
				med = (left + right)/2;
				fmed = f(med);
				if (fmed == t)
					return med;
				if (fmed < t)
					left = med + 1;
				else
					right = med - 1;
			}
		return 0;
	}

int main(void)
	{
		unsigned n, i;
		for (i = 0; i < 110; i++)
			printf("%u - %u\n", i, f(i));
		scanf("%u", &n);
		printf("%u\n", g(n));
		return 0;
	}