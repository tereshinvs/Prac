#include <stdio.h>            
#include <math.h>

unsigned f(unsigned x)
{
    return x * (unsigned)floor(sqrt(x));
}

unsigned long long root3(unsigned long long t)
{
	unsigned long long left = 0ULL, right = 2500000, med, fmed;
	while (left <= right)
	{
		med = (left+right)/2;
		fmed = med*med*med;
		if (fmed == t)
			return med;
		if (fmed < t)
			left = med + 1;
		else
			right = med - 1;
	}
	return left - 1;
}

unsigned g(unsigned x)
{   
	unsigned long long root, q, i, end_cycle;
	if (x == 0)
		return 0;
	for (q = 0; q < (1ULL << 32); q++)
	{
		root = root3(x + (q << 32)); 
		if ((x + (q << 32))%root != 0)
			continue;
		end_cycle = (root+1)*(root+1);
		for (i = root*root; i < end_cycle; i++)
			if (f(i) == x)
				return i;
	}
	return 0;
}

int main(void)
{
	unsigned n;
	scanf("%u", &n);
	printf("%u\n", g(n));
	return 0;
}