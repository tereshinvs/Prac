#include <stdio.h>

unsigned long data[1500000], alr[1500000];

int main(void)
{
	long n, i, j, flag;
	scanf("%ld", &n);
	for (i = 0; i < n; i++)
	{
		scanf("%lu", &data[i]);
		alr[i] = 0;
	}
	for (i = 0; i < n; i++)
	{
		if (alr[i])
			continue;
		alr[i] = 1;
		flag = 1;
		for (j = 0; j < n && flag; j++)
			if (j != i && data[i] == data[j] && !alr[j])
			{
				flag = 0;
				alr[j] = 1;
			}
		if (flag)
			printf("%lu ", data[i]);
	}
	return 0;
}