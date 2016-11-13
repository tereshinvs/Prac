#include <stdio.h>

int main(void)
{
	int i;
	for (i = 0; i < 1000000; i++)
		printf("a");
	printf("\n");
	return 0;
}