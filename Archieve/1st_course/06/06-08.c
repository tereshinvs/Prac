#include <stdio.h>
#include <stdlib.h>

typedef struct list list;

struct list {
	char info;
	list *next, *prev;
};

list *add(list *v, char c) {
	if (!v) {
		v = (list *)malloc(sizeof(list));
		v->info = c;
		v->next = NULL;
		v->prev = NULL;
		return v;
	}
	list *res = (list *)malloc(sizeof(list));
	v->prev = res;
	res->info = c;
	res->next = v;
	res->prev = NULL;
	return res;
}

list *delete(list *v) {
	list *pos;
	while (v) {
		pos = v->next;
		free(v);
		v = pos;
	}
	return NULL;
}

void out_list(list *v, int size, int need_space) {
	list *last = v;
	int i;
	if (need_space)
		printf(" ");
	while (last->next)
		last = last->next;
	for (i = size - 1; i >= 0; i--, last = last->prev)
		if ((size%2 && i != size / 2) || !(size%2))
			printf("%c", last->info);
}

int main(void) {
	list *l = NULL;
	int len = 0, w = 0;
	char c;
	freopen("words.in", "r", stdin);
	freopen("words.out", "w", stdout);
	while (1) {
		scanf("%c", &c);
		if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
			l = add(l, c);
			len++;
		} else {
			if (len > 1)
				out_list(l, len, w);
			w += len > 0;
			l = delete(l);
			len = 0;
		}
		if (c == '.')
			break;
	}
	printf(".\n");
	fclose(stdin);
	fclose(stdout);
	return 0;
}