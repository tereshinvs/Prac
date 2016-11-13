#include <stdio.h>

unsigned power3(unsigned n) {
	return (!n ? 1 : 3 * power3(n - 1));
}

int main(void) {
	unsigned n;
	scanf("%u", &n);
	printf("%u\n", power3(n));
	return 0;
}