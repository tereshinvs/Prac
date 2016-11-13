#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define INF 1000000000

#define set(start, x, y, value) *(start + x*m + y) = value 
#define get(start, x, y) *(start + x*m + y)
#define get_length(x1, y1, x2, y2) abs(get(field, x1, y1) - get(field, x2, y2))

typedef struct Queue Queue;

struct Queue {
	int x, y;
	Queue *next;
};

int n, m, n0, m0, n1, m1, *field, *alr, *res;
Queue *start = NULL, *last, *need_to_free = NULL;

void push(int x, int y) {
	if (get(alr, x, y))
		return;
	set(alr, x, y, 1);
	if (!last)
		last = (Queue *)malloc(sizeof(Queue));
	else
		last = last->next = (Queue *)malloc(sizeof(Queue));
	last->x = x;
	last->y = y;
	last->next = NULL;
	if (!start)
		start = last;
}              

int main(void) {
	int i, j, x, y, temp;
	scanf("%d%d%d%d%d%d", &n, &m, &n0, &m0, &n1, &m1);
	field = (int *)malloc(n * m * sizeof(int));
	alr = (int *)malloc(n * m * sizeof(int));
	res = (int *)malloc(n * m * sizeof(int));
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			scanf("%d", field + i*m + j);
			set(alr, i, j, 0);
		}
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			set(res, i, j, INF);
	set(res, n0, m0, 0);
	push(n0, m0);
	while (start) {
		x = start->x; y = start->y; set(alr, x, y, 0);
		printf("%d %d\n", x, y);
		free(need_to_free);
		need_to_free = start;
		start = start->next;
		temp = get(res, x, y);
		if (x > 0 && get(res, x - 1, y) > temp + get_length(x, y, x - 1, y)) {
			set(res, x - 1, y, temp + get_length(x, y, x - 1, y));
			push(x - 1, y);
		}
		if (x < n - 1 && get(res, x + 1, y) > temp + get_length(x, y, x + 1, y)) {
			set(res, x + 1, y, temp + get_length(x, y, x + 1, y));
			push(x + 1, y);
		}
		if (y > 0 && get(res, x, y - 1) > temp + get_length(x, y, x, y - 1)) {
			set(res, x, y - 1, temp + get_length(x, y, x, y - 1));
			push(x, y - 1);
		}
		if (y < m - 1 && get(res, x, y + 1) > temp + get_length(x, y, x, y + 1)) {
			set(res, x, y + 1, temp + get_length(x, y, x, y + 1));
			push(x, y + 1);
		}
	}
	free(need_to_free);
	printf("%d\n", get(res, n1, m1));
	free(field);
	free(alr);
	free(res);
	return 0;
}