#include <stdio.h>
int main(void)
{
	int a, b;
	if (scanf("%d%d", &a, &b) == 2)
		printf("%d\n", a + b);
	return 0;
}
