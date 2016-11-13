#include <stdio.h>
#include <string.h>

struct res_element {
	int ex, arg1, arg2, sum, prev;
};

int get_digit(char c) {
	return c == '?' ? -1 : c - '0';
}

int main(void) {
	int arg1[200], arg2[200], sum[200], i, j, q, len1, len2, lensum, plus, eqsmb, curprev;
	struct res_element res[200][2];
	char s[201], resarg1[200], resarg2[200], ressum[200];
	for (i = 0; i < 200; i++) {
		arg1[i] = 0;
		arg2[i] = 0;
		sum[i] = 0;
		res[i][0].ex = 0;
		res[i][1].ex = 0;
	}

	scanf("%s", s);
	plus = strchr(s, '+') - s; eqsmb = strchr(s, '=') - s;
	len1 = plus;
	for (i = len1 - 1; i >= 0; i--)
		arg1[len1 - i - 1] = get_digit(s[i]);
	len2 = eqsmb - plus - 1;
	for (i = eqsmb - 1; i > plus; i--)
		arg2[eqsmb - i - 1] = get_digit(s[i]);
	lensum = strlen(s) - eqsmb - 1;
	for (i = strlen(s) - 1; i > eqsmb; i--)
		sum[strlen(s) - i - 1] = get_digit(s[i]);

	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			if ((i == arg1[0] || arg1[0] == -1)
				&& (j == arg2[0] || arg2[0] == -1)
				&& ((i + j)%10 == sum[0] || sum[0] == -1)) {
				if (i + j < 10) {
					res[0][0].ex = 1;
					res[0][0].arg1 = i;
					res[0][0].arg2 = j;
					res[0][0].sum = i + j;
				} else {
					res[0][1].ex = 1;
					res[0][1].arg1 = i;
					res[0][1].arg2 = j;
					res[0][1].sum = (i + j)%10;
				}
			}

	for (q = 1; q < 200; q++)
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++) {
				if (res[q - 1][0].ex
					&& (i == arg1[q] || arg1[q] == -1)
					&& (j == arg2[q] || arg2[q] == -1)
					&& ((i + j)%10 == sum[q] || sum[q] == -1)) {
					if (i + j < 10) {
						res[q][0].ex = 1;
						res[q][0].arg1 = i;
						res[q][0].arg2 = j;
						res[q][0].sum = i + j;
						res[q][0].prev = 0;
					} else {
						res[q][1].ex = 1;
						res[q][1].arg1 = i;
						res[q][1].arg2 = j;
						res[q][1].sum = (i + j)%10;
						res[q][1].prev = 0;
					}
				}

				if (res[q - 1][1].ex
					&& (i == arg1[q] || arg1[q] == -1)
					&& (j == arg2[q] || arg2[q] == -1)
					&& ((i + j + 1)%10 == sum[q] || sum[q] == -1)) {
					if (i + j + 1 < 10) {
						res[q][0].ex = 1;
						res[q][0].arg1 = i;
						res[q][0].arg2 = j;
						res[q][0].sum = i + j + 1;
						res[q][0].prev = 1;
					} else {
						res[q][1].ex = 1;
						res[q][1].arg1 = i;
						res[q][1].arg2 = j;
						res[q][1].sum = (i + j + 1)%10;
						res[q][1].prev = 1;
					}
				}
			}

	for (i = len1 - 1; i >= lensum; i--)
		resarg1[i] = '0';
	for (i = len2 - 1; i >= lensum; i--)
		resarg2[i] = '0';

	for (i = 199, curprev = 0; i >= 0; curprev = res[i][curprev].prev, i--) {
		if (!res[i][curprev].ex) {
			printf("No\n");
			return 0;
		}
		resarg1[i] = res[i][curprev].arg1 + '0';
		resarg2[i] = res[i][curprev].arg2 + '0';
		ressum[i] = res[i][curprev].sum + '0';
	}

	for (i = len1 - 1; i >= 0; i--)
		printf("%c", resarg1[i]);
	printf("+");
	for (i = len2 - 1; i >= 0; i--)
		printf("%c", resarg2[i]);
	printf("=");
	for (i = lensum - 1; i >= 0; i--)
		printf("%c", ressum[i]);
	printf("\n");

	return 0;
}