#include <stdio.h>

int main(void) {
	int n, k, i, j;
	int a[21], b[21];
	int *esi = a, *edi = b, *temp;
	scanf("%d%d", &n, &k);
	*esi = 1;
	i = 0;
	while (i < n) {
		i++;
		*edi = 1;
		j = 0;
		while (j < i) {
			j++;
			*(edi + j) = *(esi + j) + *(esi + j - 1);
		}
		*(edi + j) = 1;
		temp = esi;
		esi = edi;
		edi = temp;
	}
	printf("%d\n", *(esi + k));
	return 0;
}