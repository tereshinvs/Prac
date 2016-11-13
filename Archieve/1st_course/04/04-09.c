#include <stdio.h>
#include <string.h>

char s[81], res[81];
int n;

int main(void)
{
	int len, i;
	scanf("%d%s", &n, s);
	len = strlen(s);
	for (i = 0; i < len; i++)
		res[((i - n)%len + len)%len] = s[i];
	printf("%s\n", res);
	return 0;
}