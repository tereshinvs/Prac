#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element element;

struct element {
	char *key;
	long long info;
};

element data[100000];
int n;

int cmp(const void *t1, const void *t2) {
	return strcmp(((element *)t1)->key, ((element *)t2)->key);
}

long long find(char *s) {
	int l = 0, r = n - 1, med, fmed;
	while (l <= r) {
		fmed = strcmp(data[med = (l + r) / 2].key, s);
		if (!fmed)
			return data[med].info;
		if (fmed < 0)
			l = med + 1;
		else
			r = med - 1;
	}
	return -1;
}

int main(void) {
	int i, m;
	char s[101];
	freopen("input.txt", "r", stdin);
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		data[i].key = (char *)malloc(101 * sizeof(char));
		scanf("%s%lld", data[i].key, &data[i].info);
	}
	qsort(data, n, sizeof(element), cmp);
	scanf("%d", &m);
	freopen("output.txt", "w", stdout);
	for (i = 0; i < m; i++) {
		scanf("%s", s);
		printf("%lld\n", find(s));
	}
	fclose(stdin);
	fclose(stdout);
	for (i = 0; i < n; i++)
		free(data[i].key);
	return 0;
}
