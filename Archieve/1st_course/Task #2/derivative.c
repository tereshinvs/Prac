#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct formula formula;

struct formula {
	char smb;
	formula *left, *right;
};

int is_op(char c) {
	return c == '+' || c == '-' || c == '*';
}

void out_formula(formula *v) {
	if (!is_op(v->smb)) {
		printf("%c", v->smb);
		return;
	}
	printf("(");
	out_formula(v->left);
	printf("%c", v->smb);
	out_formula(v->right);
	printf(")");
}

void out_derivative(formula *v) {
	if (v->smb == '+' || v->smb == '-') {
		printf("(");
		out_derivative(v->left);
		printf("%c", v->smb);
		out_derivative(v->right);
		printf(")");
	}
	if (v->smb == '*') {
		printf("(");
		out_derivative(v->left);
		printf("*");
		out_formula(v->right);
		printf("+");
		out_formula(v->left);
		printf("*");
		out_derivative(v->right);
		printf(")");
	}
	if (v->smb == 'x') {
		printf("1");
		return;
	}
	if (isdigit(v->smb))
		printf("0");
}

void free_tree(formula *v) {
	if (!v)
		return;
	free_tree(v->left);
	free_tree(v->right);
	free(v);
}

formula *input_formula();

formula *next_formula_part() {
	formula *res = NULL;
	char c = getchar();
	switch (c) {
		case '(':
			res = input_formula();
			if (getchar() != ')') {
				free_tree(res);
				return NULL;
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'x':
			res = (formula *)malloc(sizeof(formula));
			res->smb = c;  res->left = res->right = NULL;
			break;
		default:
			break;
	}
	return res;
}

formula *input_formula() {
	static int dep = 0;
	formula *res = NULL;
	if (!dep++) {
		res = next_formula_part();
		if (!res || getchar() != '\n') {
			free_tree(res);
			return NULL;
		}
		dep = 0;
		return res;
	}
	res = (formula *)malloc(sizeof(formula)); res->left = res->right = NULL;
	res->left = next_formula_part();
	char c = getchar();
	if (c == '+' || c == '-' || c == '*')
		res->smb = c;
	else {
		free_tree(res);
		return NULL;
	}
	res->right = next_formula_part();
	if (!res->left || !res->right) {
		free_tree(res);
		return NULL;
	}
	return res;
}

int main(void) {
	printf("f=");
	formula *f_tree = input_formula();
	if (f_tree) {
		printf("f'="); out_derivative(f_tree); printf("\n");
	} else
		printf("Incorrect formula\n");
	free_tree(f_tree);
	return 0;
}