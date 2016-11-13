#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int l;
	char *s1, *s2, *s3;
	scanf("%d", &l);
	s1 = (char *)malloc((l + 1) * sizeof(char));
	s2 = (char *)malloc((l + 1) * sizeof(char));
	s3 = (char *)malloc((l + 1) * sizeof(char));
	scanf("%s%s%s", s1, s2, s3);
	printf("%s%s%s\n", s3, s1, s2);
	free(s1); free(s2); free(s3);
	return 0;
}