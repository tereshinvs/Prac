#include <stdio.h>

int n, temp;

int check_symmetric(int depth)
	{
		int x1, y1, x2, y2;
		scanf("%d%d", &x1, &y1);
		if (depth < n/2 - 1)
			{
				if (!check_symmetric(depth + 1))
					{
						scanf("%d%d", &x2, &y2);
						return 0;
					}
			} else
				return 1;
		scanf("%d%d", &x2, &y2);
		return x1 == -x2 && y1 == y2;
	}

int main(void)
	{
		scanf("%d%d%d", &n, &temp, &temp);
		printf("%s\n", check_symmetric(0) ? "Yes" : "No");
		return 0;
	}