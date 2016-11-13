#include <stdio.h>

unsigned f(unsigned x)
{
    long long s = 0;
    unsigned i;
    for (i = 0; s <= x; i++) {
        s += 2 * i + 1;
    }
	printf("x=%u s=%lld i=%u\n", x, s, i);
	printf("%u\n", x*(i-1));
    return x * (i - 1);
}

int main(void)
{
	unsigned x;
	printf("%u\n", (unsigned)0 - 1);
	while (1)
	{
		scanf("%u", &x);
		f(x);
	}
	return 0;
}