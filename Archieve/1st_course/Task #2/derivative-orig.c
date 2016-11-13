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

int get_prior(char c) {
	if (c == '*')
		return 2;
	return c == '-';
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

void input_formula(int *lng, char **f) {
	*lng = 0;
	char c;
	int cur_f_lng = 10;
	*f = (char *)malloc(10);
	while ((c = getchar()) != '\n') {
		if (*lng == cur_f_lng) {
			cur_f_lng *= 2;
			*f = (char *)realloc(*f, cur_f_lng);
		}
		(*f)[(*lng)++] = c;
	}
	(*f)[*lng] = 0;
}

int is_correct_bracket_balance(char *f, int lng) {
	int i, bal = 0;
	for (i = 0; i < lng; i++) {
		if (f[i] == '(')
			bal++;
		if (f[i] == ')')
			bal--;
		if (bal < 0)
			return 0;
	}
	return bal == 0;
}

void verify_formula(char *f, int lng) {
	int i;
	if (lng == 0) {
		printf("Formula must not be empty\n");
		exit(0);
	}
	for (i = 0; i < lng; i++)
		if (f[i] != '*' && f[i] != '+' && f[i] !='-' &&
			!isdigit(f[i]) && f[i] != 'x' &&
			f[i] != '(' && f[i] != ')') {
			printf("Unknown symbol \"%c\" at position %d\n", f[i], i + 1);
			exit(0);
		}

	if (!is_correct_bracket_balance(f, lng)) {
		printf("\"(\" or \")\" is missed\n");
		exit(0);
	}

	for (i = 0; i < lng - 1; i++) {
		if (is_op(f[i]) && (is_op(f[i + 1]) || f[i + 1] == ')')) {
			printf("Incorrect formula at postition %d\n", i + 1);
			exit(0);
		}
		if ((isdigit(f[i]) || f[i] == 'x') &&
			(isdigit(f[i + 1]) || f[i + 1] == 'x' || f[i + 1] == '(')) {
			printf("Incorrect formula at positition %d\n", i + 1);
			exit(0);
		}
		if ((f[i] == ')' && !is_op(f[i + 1])) || (f[i] == '(' && is_op(f[i + 1]))) {
			printf("Incorrect formula at postition %d\n", i + 1);
			exit(0);
		}
	}
	if (is_op(f[lng - 1])) {
		printf("Incorrect formula at position %d\n", lng);
		exit(0);
	}
	if (is_op(f[0])) {
		printf("Incorrect formula at position 1\n");
		exit(0);
	}
}

formula *construct_formula_tree(char *f_str, int lng) {
	int min_prior = 100, pos, i, bal = 0;
	while (f_str[0] == '(' && f_str[lng - 1] == ')' &&
			is_correct_bracket_balance(f_str + 1, lng - 2)) {
		f_str++;
		lng -= 2;
	}
	for (i = 0; i < lng; i++) {
		if (f_str[i] == '(')
			bal++;
		if (f_str[i] == ')')
			bal--;
		if (!bal && is_op(f_str[i]) && get_prior(f_str[i]) < min_prior) {
			min_prior = get_prior(f_str[i]);
			pos = i;
		}
	}
	formula *res = (formula *)malloc(sizeof(formula));
	if (min_prior < 100) {
		res->smb = f_str[pos];
		res->left = construct_formula_tree(f_str, pos);
		res->right = construct_formula_tree(f_str + pos + 1, lng - pos - 1);
	} else {
		res->smb = *f_str;
		res->left = res->right = NULL;
	}
	return res;
}

void free_tree(formula *v) {
	if (!v)
		return;
	free_tree(v->left);
	free_tree(v->right);
	free(v);
}

int main(void) {
	int lng;
	char *f_str;
	formula *f_tree;
	printf("f="); input_formula(&lng, &f_str);
	verify_formula(f_str, lng);
	f_tree = construct_formula_tree(f_str, lng);
	out_formula(f_tree); printf("\n");
	printf("f'="); out_derivative(f_tree); printf("\n");
	free(f_str);
	free_tree(f_tree);
	return 0;
}