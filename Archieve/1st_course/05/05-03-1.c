#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char **t1, char **t2) {
	char *temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

int cmp(char *t1, char *t2) {
	int i;
	for (i = 0; t1[i] || t2[i]; i++) {
		if ('a' <= t1[i] && t1[i] <= 'z' && 'A' <= t2[i] && t2[i] <= 'Z')
			return -1;
		if ('A' <= t1[i] && t1[i] <= 'Z' && 'a' <= t2[i] && t2[i] <= 'z')
			return 1;
		if (t1[i] - t2[i])
			return t1[i] - t2[i];
	}
	return 0;
}

void sort(char **left, char **right) {
	char **il = left, **ir = right, **imed = left + rand()%(right - left + 1);
	char *med = *imed;
	strcpy(med, *imed);
	do {
		while (cmp(*il, med) < 0)
			il++;
		while (cmp(*ir, med) > 0)
			ir--;
		if (il <= ir)
			swap(il++, ir--);
	} while (il <= ir);
	if (left < ir)
		sort(left, ir);
	if (il < right)
		sort(il, right);
}

int main(void) {
	int n, i;
	char *s[500000], *temp = (char *)malloc(sizeof(char) * 9000001);
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		scanf("%s", temp);
		s[i] = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
		strcpy(s[i], temp);
	}
	free(temp);
	sort(s, s + n - 1);
	for (i = 0; i < n; i++) {
		printf("%s\n", s[i]);
		free(s[i]);
	}
	return 0;
}