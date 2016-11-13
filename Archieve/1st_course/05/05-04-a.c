#include <stdio.h>
#include <stdlib.h>

typedef struct List List;

struct List {
	int val;
	List *next;
};

List *add_element(int value) {
	List *res = (List *)malloc(sizeof(List));
	if (!res) {
		printf("Memory\n");
		exit(0);
	}
	res->val = value;
	res->next = NULL;
	return res;
}

int main(void) {
	int q, i, temp;
	List *root = add_element(-1), *last = root, *cur, *curtemp;
	while (1) {
		scanf("%d", &temp);
		if (!temp)
			break;
		last = (last->next = add_element(temp));
	}
	for (q = 0; q <= 1; q++)
		for (i = 0, cur = root->next; cur; i++, cur = cur->next)
			if (i%2 == q)
				printf("%d ", cur->val);
	printf("\n");
	for (cur = root; cur;) {
		curtemp = cur->next;
		free(cur);
		cur = curtemp;
	}
	return 0;
}