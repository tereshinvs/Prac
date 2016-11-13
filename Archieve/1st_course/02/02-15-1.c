#include <stdio.h>

unsigned f(unsigned x)
	{
   	long long s = 0;
   	unsigned i;
    	for (i = 0; s <= x; i++)
    		s += 2 * i + 1;
	   return x * (i - 1);
	}

unsigned long long root3(unsigned long long t)
	{
		unsigned long long i;
		for (i = 0; i < 1626; i++)
			if (i*i*i > t)
				return i - 1;
		return 1625;
	}

unsigned long long g(unsigned long long t)
	{
		unsigned long long root = root3(t);       
		return root*root + (t - root*root*root)/root;
	}

int main(void)
	{
		unsigned long long n;
		int i;
		scanf("%llu", &n);
		if (n == 0)
			printf("0\n");
		else {
			if (f(g(n)) == n) {
				printf("%llu\n", g(n));
				return 0;
				}
			for (i = 31; i < 64; i++)
				if (f(g(n << i)) == n) {
					printf("%llu\n", g(n << i));
					return 0;
					}
			}
		return 0;
	}