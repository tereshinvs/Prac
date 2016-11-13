#include <stdio.h>

int main(void)
	{
		int k;
		char c;
		scanf("%d", &k);
		do
			{
				scanf("%c", &c);
				if ('A' <= c && c <= 'Z')
					printf("%c", (c - 'A' + k)%26 + 'A');
				else if ('a' <= c && c <= 'z')
					printf("%c", (c - 'a' + k)%26 + 'a');
				else printf("%c", c);
			} while (c != '.');
		return 0;
	}