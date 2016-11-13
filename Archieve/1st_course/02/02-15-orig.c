#include <stdio.h>

long long ts = 0;
unsigned ti = 0;

unsigned f(unsigned x)
	{
    	long long s = ts;
    	unsigned i;
		if (s + 2*(ti + 1) + 1 > x)
			return x*(ti - 1);
    	for (i = ti + 1; s <= x; i++)
      	s += 2 * i + 1;
		ts = s;
		ti = i;
    	return x * (i - 1);
	}

unsigned g(unsigned n)
	{
		unsigned i;
		for (i = 0; f(i) != n; i++);
		return i;
	}

int main(void)
	{
		unsigned n;
		scanf("%u", &n);
		printf("%u\n", g(n));
		return 0;
	}