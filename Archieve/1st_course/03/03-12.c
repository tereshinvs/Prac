#include <stdio.h>

int main(void)
{
	int n1, n2, data1[50000], data2[50000], i;
	scanf("%d", &n1);
	for (i = 0; i < n1; i++)
		scanf("%d", &data1[i]);
	scanf("%d", &n2);
	for (i = 0; i < n2; i++)
		scanf("%d", &data2[i]);
	for (i = 0; i < n1 && i < n2; i++)
		printf("%d %d ", data1[i], data2[i]);
	if (i < n1)
		for (; i < n1; i++)
			printf("%d ", data1[i]);
	else
		for (; i < n2; i++)
			printf("%d ", data2[i]);
	return 0;
}