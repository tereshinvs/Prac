#include <stdio.h>

char a[1000], c;

int main(void)
{
	int i, j, n, left_sum = 0, right_sum = 0;
	for (n = 0; (c = getchar()) != '\n'; n++)
		a[n] = c;           
	if (n%2)
	{
		for (i = n; i > 0; i--)
			a[i] = a[i - 1];
		a[0] = '0';
		n++;
	}                      
	for (i = 0; i < n/2; i++)
		left_sum += a[i];
	for (i = n/2; i < n; i++)
		right_sum += a[i];
	if (left_sum >= right_sum)
	{
first_case:
		for (i = n - 1; i >= 0; i--)
			if ('9' - a[i] > left_sum - right_sum)
			{
				a[i] += left_sum - right_sum;
				break;
			} else {
				right_sum += '9' - a[i];
				a[i] = '9';
			}
		//if (a[0] != '0')
		//	printf("%c", a[0]);
		for (i = 0; i < n; i++)
			printf("%c", a[i]);
		printf("\n");
	} else {
		for (i = n - 1; i >= 0; i--)
		{
			if (a[i] == '0')
				continue;
			a[i] = '0';
			for (j = i - 1; j >= 0; j--)
				if (a[j] < '9')
				{
					a[j]++;
					break;
				} else
					a[j] = '0';
			left_sum = 0;
			for (j = 0; j < n/2; j++)
				left_sum += a[j];
			right_sum = 0;
			for (j = n/2; j < n; j++)
				right_sum += a[j];
			if (left_sum >= right_sum)
				break;
		}
		goto first_case;
	}
	return 0;
}