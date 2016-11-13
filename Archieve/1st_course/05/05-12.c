#include <stdio.h>
#include <stdlib.h>

#define MAT_SIZE 20

typedef struct Matrix Matrix;

struct Matrix {
	long long mat[MAT_SIZE][MAT_SIZE];
	int n, m;
};

int p;

void copy_mat(Matrix *dst, Matrix *src) {
	int i, j;
	dst->n = src->n;
	dst->m = src->m;
	for (i = 0; i < MAT_SIZE; i++)
		for (j = 0; j < MAT_SIZE; j++)
			dst->mat[i][j] = src->mat[i][j];
}


void mult_mat(Matrix *t1, Matrix *t2, Matrix *res) {
	int i, j, q;
	res->n = t1->n;
	res->m = t2->m;
	for (i = 0; i < res->n; i++)
		for (j = 0; j < res->m; j++) {
			res->mat[i][j] = 0;
			for (q = 0; q < t1->m; q++)
				res->mat[i][j] = (res->mat[i][j] + t1->mat[i][q] * t2->mat[q][j]) % p;
		}
}

void power(Matrix *t, int pwr, Matrix *dst) {
	Matrix *temp = (Matrix *)malloc(sizeof(Matrix)),
		*temp2 = (Matrix *)malloc(sizeof(Matrix));
	if (pwr == 1) {
		copy_mat(dst, t);
		free(temp); free(temp2);
		return;
	}
	power(t, pwr/2, temp);
	mult_mat(temp, temp, temp2);
	if (pwr%2)
		mult_mat(temp2, t, dst);
	else
		copy_mat(dst, temp2);
	free(temp); free(temp2);
}

int main(void) {
	Matrix a, b, c, res;
	int i, k, n, j;
	scanf("%d%d%d", &k, &n, &p);
	a.n = k; a.m = 1;
	for (i = 0; i < k; i++) {
		scanf("%lld", &a.mat[k - i - 1][0]);
		a.mat[k - i - 1][0] %= p;
	}
	b.n = k; b.m = k;
	for (i = 0; i < k; i++)
		for (j = 0; j < k; j++)
			b.mat[i][j] = 0;
	for (i = 1; i < k; i++)
		b.mat[i][i - 1] = 1 % p;
	for (i = 0; i < k; i++) {
		scanf("%lld", &b.mat[0][i]);
		b.mat[0][i] %= p;
	}
	if (n <= k) {
		printf("%lld\n", a.mat[k - n][0]);
		return 0;
	}
	power(&b, n - k, &c);
	mult_mat(&c, &a, &res);
	printf("%lld\n", res.mat[0][0]);
	return 0;
}