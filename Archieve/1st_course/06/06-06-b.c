#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 20000000

typedef struct list list;

struct list {
	long long data;
	list *next;
};

list *add(list *v, long long t) {
	if (!v) {
		v = (list *)malloc(sizeof(list));
		v->data = t;
		v->next = NULL;
		return v;
	}
	list *res = (list *)malloc(sizeof(list));
	res->data = t;
	res->next = v;
	return res;
}

list *pop(list *v) {
	if (!v)
		return NULL;
	list *res = v->next;
	free(v);
	return res;
}

long long get_number(char *t) {
	long long res = 0;
	while (isdigit(*t))
		res = res * 10 + (*(t++) - '0');
	return res;
}

int prior(char c) {
	if (c == '(')
		return -1;
	if (c == 'u')
		return 2;
	return c == '*' || c =='/';
}

int is_unary(char *s, int i) {
	i--;
	while (i >= 0) {
		if (isdigit(s[i]) || s[i] == ')' || s[i] == '+' || s[i] == '/' || s[i] == '-' || s[i] == '*')
			return 0;
		if (s[i] == '(')
			return 1;
		i--;
	}
	return 1;
}

int is_empty(char *s) {
	int i;
	for (i = 0; s[i] != 0; i++)
		if (s[i] != ' ')
			return 0;
	return 1;
}

char s[MAX_LENGTH], rev_pol_not[MAX_LENGTH], stemp[MAX_LENGTH];

int main(void) {
	long long k = 0, len = 0, i, arg1, arg2, temp;
	list *st = NULL;
	char c;
	freopen("input.txt", "r", stdin);
	gets(stemp);
	temp = strlen(stemp);
	for (i = 0; i < temp; i++) {
		c = stemp[i];
		if ((c == '+' && !is_unary(stemp, i)) || (c == '-' && !is_unary(stemp, i))
			|| c == '*' || c == '/' || c == '(' || c == ')') {
			s[len++] = ' ';
			s[len++] = c;
			s[len++] = ' ';
		} else {
			if (c == '-' && is_unary(stemp, i))
				s[len++] = 'u';
			else
				if (c != '+' || !is_unary(stemp, i))
					s[len++] = c;
		}
	}
	s[len] = 0;
	if (is_empty(s)) {
		freopen("output.txt", "w", stdout);
		printf("0\n");
		fclose(stdin);
		fclose(stdout);
		return 0;
	}
	fclose(stdin);


	for (i = 0; i < len; i++) {
		if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == 'u') {
			while (st && prior(st->data) >= prior(s[i])) {
				rev_pol_not[k++] = st->data;
				st = pop(st);
			}
			st = add(st, s[i]);
			continue;
		}
		if (s[i] == '(') {
			st = add(st, s[i]);
			continue;
		}
		if (s[i] == ')') {
			while (st && st->data != '(') {
				rev_pol_not[k++] = st->data;
				st = pop(st);
			}
			st = pop(st);
			continue;
		}
		rev_pol_not[k++] = s[i];
	}
	while (st) {
		rev_pol_not[k++] = st->data;
		st = pop(st);
	}
	rev_pol_not[k] = 0;

	st = NULL;
	freopen("output.txt", "w", stdout);
	for (i = 0; i < k; i++) {
		if (rev_pol_not[i] == ' ')
			continue;
		if (isdigit(rev_pol_not[i])) {
			st = add(st, get_number(rev_pol_not + i));
			while (isdigit(rev_pol_not[i]))
				i++;
			i--;
			continue;
		}
		arg1 = st->data; st = pop(st);
		if (rev_pol_not[i] == 'u') {
			st = add(st, -arg1);
			continue;
		}
		arg2 = st->data; st = pop(st);
		switch (rev_pol_not[i]) {
			case '+': st = add(st, arg2 + arg1); break;
			case '-': st = add(st, arg2 - arg1); break;
			case '*': st = add(st, arg2 * arg1); break;
			case '/': st = add(st, arg2 / arg1); break;
			default: st = add(st, arg2); st = add(st, arg1); break;
		}
	}

	printf("%lld", st->data);
	fclose(stdout);
	pop(st);
	return 0;
}