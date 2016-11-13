#include <stdio.h>

int main(void) {
	unsigned int n, flag = 0;
	scanf("%u", &n);
	n ^= (n - 1);
	flag = !~n;
	n += 1;
	n >>= 1;
	n += flag << 31;
	printf("%u\n", n);
	return 0;
}