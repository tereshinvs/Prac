#include <stdio.h>

int gcd(int n, int m) {
//	return n ? gcd(m % n, n) : m;
	while (n) {
		int temp = n;
		n = m % n;
		m = temp;
	}
	return m;
}

int main(void) {
	int n, m;
	scanf("%d%d", &n, &m);
	printf("%d\n", gcd(n, m));
	return 0;
}