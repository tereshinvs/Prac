#include <stdio.h>

int main(void)
	{
		int n, cur, last, max_len = 1, cur_len = 1, i;
		scanf("%d%d", &n, &last);
		for (i = 1; i < n; i++)
			{
				scanf("%d", &cur);
				if (cur > last)
					cur_len++;
				else
					cur_len = 1;
				if (cur_len > max_len)
					max_len = cur_len;
				last = cur;
			}
		printf("%d\n", max_len);
		return 0;
	}