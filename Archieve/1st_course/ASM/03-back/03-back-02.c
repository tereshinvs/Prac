#include <stdio.h>

int main(void) {
	int a, b, res = 0;
	char op;

	scanf("%d%c%d", &a, &op, &b);

	switch (op) {
		case '+':
			res = a + b;
			break;
		case '-':
			res = a - b;
			break;
		case '*':
			res = a * b;
			break;
		case '/':
			res = a / b;
			break;
		default:
			break;
	}

	printf("%d\n", res);

	return 0;
}