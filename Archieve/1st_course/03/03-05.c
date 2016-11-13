#include <stdio.h>

int min(int t1, int t2)
{
	return t1 < t2 ? t1 : t2;
}

int max(int t1, int t2)
{
	return t1 > t2 ? t1 : t2;
}

int main(void)
{
	int front[8], left[8], i, max_front[8], max_left[8], res = 0, temp, j;
	for (i = 0; i < 8; i++)
	{
		scanf("%d", &front[i]);
		max_front[i] = 0;
	}
	for (i = 0; i < 8; i++)
	{
		scanf("%d", &left[i]);
		max_left[i] = 0;
	}	
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			temp = min(front[i], left[j]);
			max_front[i] = max(max_front[i], temp);
			max_left[j] = max(max_left[j], temp);
			res += temp;
		}
	for (i = 0; i < 8; i++)
		if (front[i] != max_front[i] || left[i] != max_left[i])
		{
			printf("-1\n");
			return 0;
		}
	printf("%d\n", res);
	return 0;
}