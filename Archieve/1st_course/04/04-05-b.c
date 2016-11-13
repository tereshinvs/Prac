#include <stdio.h>
#include <string.h>

long min(long t1, long t2)
{
	return t1 < t2 ? t1 : t2;
}

void z_func(char *s, long *z_res)
{
	long n = strlen(s), l, r, i;
	z_res[0] = 0;
	for (i = 1, l = 0, r = 0; i < n; i++)
	{
		if (i <= r)
			z_res[i] = min(r - i + 1, z_res[i - l]);
		else
			z_res[i] = 0;
		while (i + z_res[i] < n && s[z_res[i]] == s[i + z_res[i]])
			z_res[i]++;
		if (i + z_res[i] - 1 > r)
			l = i, r = i + z_res[i] - 1;
	}
}

long z_res[1000002];
char s[1000002];

int main(void)
{
	long i, j, flag, n;
	scanf("%s", s);
	z_func(s, z_res);
	n = strlen(s);
	for (i = 1; i <= n; i++)
	{
		flag = 1;
		for (j = i; j < n && flag; j += i)
			if (j + i >= n)
			{
				if (z_res[j] < n - j)
					flag = 0;
			} else
				if (z_res[j] < i)
					flag = 0;
		if (flag)
			printf("%ld ", i);
	}
	printf("\n");
	return 0;
}