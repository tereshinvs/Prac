#include <stdio.h>

#define get_bit(a, b) ((a >> b)%2)

int main(void)
	{
		unsigned long a11, a12, a21, a22, b1, b2, sol_exist, i, xi, yi, x = 0, y = 0;
		scanf("%lu%lu%lu%lu%lu%lu", &a11, &a12, &a21, &a22, &b1, &b2);
		for (i = 0; i < sizeof(long)*8; i++)
			{
				sol_exist = 0;
				for (xi = 0; xi <= 1; xi++)
					for (yi = 0; yi <= 1; yi++)
						if (((get_bit(a11, i) & xi) ^ (get_bit(a12, i) & yi)) == get_bit(b1, i)
							&& ((get_bit(a21, i) & xi) ^ (get_bit(a22, i) & yi)) == get_bit(b2, i))
							{
								sol_exist = 1;
								goto solution;
							}
				if (!sol_exist)
					{
						printf("No\n");
						return 0;
					}
solution:
				x += xi << i;
				y += yi << i;
			}
		printf("Yes\n%lu %lu\n", x, y);
		return 0;
	}