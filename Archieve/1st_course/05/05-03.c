#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define get_number(t) (t < 'a') ? (t - 'A') : (t - 'a' + 26)
#define get_char(t) (t < 26) ? (t + 'A') : (t - 26 + 'a')

typedef struct Node Node;

struct Node {
	int end;
	Node *next[52];
};

Node *add_node(void) {
	int i;
	Node *res = (Node *)malloc(sizeof(Node));
	if (!res) {
		printf("Memory :(\n");
		exit(0);
	}
	res->end = 0;
	for (i = 0; i < 52; i++)
		res->next[i] = NULL;
	return res;
}

void add(Node *v) {
	char c;
	while ((c = getchar()) != '\n') {
		if (!v->next[get_number(c)])
			v->next[get_number(c)] = add_node();
		v = v->next[get_number(c)];
	}
	v->end++;
}

char out_str[9000000];

void out(Node *v, int depth) {
	int i, j;
	for (i = 0; i < v->end; i++) {
		for (j = 0; j < depth; j++)
			printf("%c", out_str[j]);
		printf("\n");
	}
	for (i = 0; i < 52; i++)
		if (v->next[i]) {
			out_str[depth] = get_char(i);
			out(v->next[i], depth + 1);
			}
	free(v);
}

int main(void) {
	Node *root = add_node();
	int n, i;
	scanf("%d", &n);
	while (getchar() != '\n');
	for (i = 0; i < n; i++) {
/*		for (j = 0; (s[j] = getchar()) != '\n'; j++);       
	//	printf("%d\n", j);*/
		add(root);
	}
	out(root, 0);
	return 0;
}