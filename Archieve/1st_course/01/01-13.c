#include <stdio.h>

#define rotate_sign(x1, y1, x2, y2, x3, y3) ((x1 - x2)*(y3 - y2) - (y1 - y2)*(x3 - x2) < 0 ? -1 : 1)

int main(void)
	{
		struct
			{
				int x, y;
			} p1, p2, cur, last, last_last;
		int n, sign, i;
		scanf("%d", &n);
		scanf("%d%d%d%d%d%d", &p1.x, &p1.y, &p2.x, &p2.y, &cur.x, &cur.y);
		last_last = p1; last = p2;
		sign = rotate_sign(last_last.x, last_last.y, last.x, last.y, cur.x, cur.y);
		for (i = 3; i < n; i++)
			{
				last_last = last; last = cur;
				scanf("%d%d", &cur.x, &cur.y);
				if (rotate_sign(last_last.x, last_last.y, last.x, last.y, cur.x, cur.y) != sign)
					{
						printf("No\n");
						return 0;
					}
			}
		if (rotate_sign(last.x, last.y, cur.x, cur.y, p1.x, p1.y) != sign
			|| rotate_sign(cur.x, cur.y, p1.x, p1.y, p2.x, p2.y) != sign)
			{
				printf("No\n");
				return 0;
		} else
			printf("Yes\n");
		return 0;
	}