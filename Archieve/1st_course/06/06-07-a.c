#include <stdio.h>
#include <stdlib.h>

typedef struct list list;

struct list {
	int data;
	list *prev, *next;
};

list *add(list *v, int t) {
	if (!v) {
		v = (list *)malloc(sizeof(list));
		v->data = t;
		v->next = NULL;
		v->prev = NULL;
		return v;
	}
	list *res = (list *)malloc(sizeof(list));
	res->data = t;
	res->next = v;
	res->prev = NULL;
	v->prev = res;
	return res;
}

int get_nth_element(list *ptr, int n) {
	int i;
	for (i = 0; i < n; i++)
		ptr = ptr->next;
	return ptr->data;
}

void swap(list *t1, list *t2) {
	int temp = t1->data;
	t1->data = t2->data;
	t2->data = temp;
}

void sort(list *start_ptr, list *end_ptr, int n) {
	list *left_ptr = start_ptr, *right_ptr = end_ptr;
	int il = 0, ir = n - 1, med = get_nth_element(start_ptr, rand()%n);
	while (il <= ir) {
		while (left_ptr->data < med)
			left_ptr = left_ptr->next, il++;
		while (right_ptr->data > med)
			right_ptr = right_ptr->prev, ir--;
		if (il <= ir) {
			swap(left_ptr, right_ptr);
			left_ptr = left_ptr->next;
			right_ptr = right_ptr->prev;
			il++; ir--;
		}
	}
	if (ir > 0)
		sort(start_ptr, right_ptr, ir + 1);
	if (il < n - 1)
		sort(left_ptr, end_ptr, n - il);
}

int find(list *v, int t) {
	while (v && v->data < t)
		v = v->next;
	return v && v->data == t;
}

void free_all(list *l) {
	list *v;
	while (l) {
		v = l->next;
		free(v);
		l = v;
	}
}

int main(void) {
	list *l1 = NULL, *end_l1 = NULL, *l2 = NULL, *end_l2 = NULL, *v;
	int n1, n2, temp, i;
	freopen("input.txt", "r", stdin);
	for (n1 = 0;; n1++) {
		scanf("%d", &temp);
		if (temp == -1)
			break;
		l1 = add(l1, temp);
		if (!n1)
			end_l1 = l1;
	}
	for (n2 = 0;; n2++) {
		scanf("%d", &temp);
		if (temp == -1)
			break;
		l2 = add(l2, temp);
		if (!n2)
			end_l2 = l2;
	}
	fclose(stdin);

	sort(l2, end_l2, n2);

	freopen("output.txt", "w", stdout);
	for (i = 0, v = end_l1; i < n1; i++, v = v->prev)
		if (!find(l2, v->data))
			printf("%d ", v->data);
	printf("\n");
	fclose(stdout);
	free_all(l1);
	free_all(l2);
	return 0;
}