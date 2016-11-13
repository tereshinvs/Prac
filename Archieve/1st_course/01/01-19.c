#include <stdio.h>

enum { matrix_size = 4 };

int main(void)
	{
		int mat[4], i, j, temp;
		char c;
		for (i = 0; i < matrix_size; i++)
			{
				scanf("%c", &c);
				mat[i] = (c>='0' && c<='9') ? (c - '0') : (c - 'a' + 10);
			}
		for (i = 0; i < matrix_size; i++)
			{
				temp = 0;
				for (j = 0; j < matrix_size; j++)
					{
						temp = temp*2 + mat[j]/(1 << (matrix_size - 1 - i));
						mat[j] %= 1 << (matrix_size - 1 - i);
					}
				printf("%c", temp < 10 ? (temp + '0') : (temp - 10 + 'a'));
			}
		printf("\n");
		return 0;
	}