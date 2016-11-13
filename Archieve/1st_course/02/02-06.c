#include <stdio.h>

enum { FIELD_SIZE = 8 };

int abs(int t)
	{
		return t > 0 ? t : -t;
	}

int too_long_game(int last_x, int last_y)
	{
		return last_x == FIELD_SIZE - 1 && last_y == FIELD_SIZE - 1;
	}

int same_cell(int last_x, int last_y, int cur_x, int cur_y)
	{
		return last_x == cur_x && last_y == cur_y;
	}

int garik_cheater(int last_x, int last_y, int cur_x, int cur_y)
	{
		return abs(last_x - cur_x) != abs(last_y - cur_y) && last_x != cur_x && last_y != cur_y;
	}

int tolik_cheater(int last_x, int last_y, int cur_x, int cur_y)
	{
		return abs(last_x - cur_x) + abs(last_y - cur_y) != 3 || last_x == cur_x || last_y == cur_y;
	}

int game_over(int last_x, int last_y)
	{
		return last_x == FIELD_SIZE - 1 && last_y == FIELD_SIZE - 1;
	}

int main(void)
	{
		int last_x = 0, last_y = 0, cur_x, cur_y, i;
		char c1, c2;
		for (i = 0, c1 = getchar(), c2 = getchar();
				c1 != 'X';
				getchar(), c1 = getchar(), c2 = getchar(), i++)
			{
				if (too_long_game(last_x, last_y))
					{
						printf("Cheaters\n");
						return 0;
					}
				cur_x = c1 - 'A';
				cur_y = c2 - '1';
				if (same_cell(last_x, last_y, cur_x, cur_y))
					{
						printf("Cheaters\n");
						return 0;
					}
				if (i%2)
					{
						if (garik_cheater(last_x, last_y, cur_x, cur_y))
							{
								printf("Cheaters\n");
								return 0;
							}
					} else {
						if (tolik_cheater(last_x, last_y, cur_x, cur_y))
							{
								printf("Cheaters\n");
								return 0;
							}
					}
				last_x = cur_x;
				last_y = cur_y;
			}
		if (game_over(last_x, last_y))
			{
				printf("%s\n", i%2 ? "Garik" : "Tolik");
				return 0;
			}
		printf("Cheaters\n");
		return 0;
	}