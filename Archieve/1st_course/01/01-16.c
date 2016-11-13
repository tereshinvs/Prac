#include <stdio.h>

int main(void)
	{
		int n, res, i, temp;
		scanf("%d%d", &n, &res);
		for (i = 1; i < n; i++)
			{
				scanf("%d", &temp);
				res ^= temp;
			}
		printf("%d\n", res);
		return 0;
	}