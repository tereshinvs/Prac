#include <stdio.h>

int main(void)
{
	int n, alr[10000], i, x;
	scanf("%d", &n);
	for (i = 0; i < n; i++)
		alr[i] = 0;
	for (i = 0; i < n; i++)
	{
		scanf("%d", &x);
		if (x > 0 && x <= n)
			alr[x - 1]++;
	}     
	for (i = 0; i < n; i++)
		if (alr[i] != 1)
		{
			printf("No\n");
			return 0;
		}
	printf("Yes\n");
	return 0;
}