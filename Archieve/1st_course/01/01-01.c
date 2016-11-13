#include <stdio.h>

int main(void)
	{
		int n, i, maximum, minimum, cur;
		scanf("%d%d", &n, &maximum);
		minimum = maximum;
		for (i = 1; i < n; i++)
			{
				scanf("%d", &cur);
				if (cur < minimum)
					minimum = cur;
				if (cur > maximum)
					maximum = cur;
			}
		printf("%d\n", maximum - minimum);
		return 0;
	}