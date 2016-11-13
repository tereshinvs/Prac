#include <stdio.h>

int main(void)
{
	int a[10000], args[10000], n, m, i, j, res, x;
	scanf("%d", &n);
	for (i = 0; i < n; i++)
		scanf("%d", &a[i]);
	scanf("%d", &m);
	for (i = 0; i < m; i++)
		scanf("%d", &args[i]);
	for (i = m - 1; i >= 0; i--)
		{
			res = 0;
			for (j = 0, x = 1; j < n; j++, x *= args[i])
				res += a[j] * x;
			printf("%d ", res);
		}
	return 0;
}