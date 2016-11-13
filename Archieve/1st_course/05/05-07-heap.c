#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define HEAP_SIZE 1100000
#define INF INT_MAX/2

typedef struct heap_element heap_element;

struct heap_element {
	int x, y, res;
};

heap_element heap[HEAP_SIZE];
int heap_len = 1, *field, *heap_pos, *res, n, m, n0, m0, n1, m1;

void set(int *dst, int x, int y, int new_value) {
	*(dst + x*m + y) = new_value;
}

int get(int *src, int x, int y) {
	return *(src + x*m + y);
}

void heap_swap(int t1, int t2) {
	set(heap_pos, heap[t1].x, heap[t1].y, t2);
	set(heap_pos, heap[t2].x, heap[t2].y, t1);
	heap_element temp = heap[t1]; heap[t1] = heap[t2]; heap[t2] = temp;
}

void push_down(int v) {
	int child;
	while (v < heap_len / 2) {
		child = 2 * v + 1;
		if (child + 1 < heap_len && heap[child].res > heap[child + 1].res)
			child++;
		if (heap[v].res <= heap[child].res)
			break;
		heap_swap(v, child);
		v = child;
	}
}

void push_up(int v) {
	int parent;
	while (v > 0) {
		parent = (v - 1) / 2;
		if (heap[v].res >= heap[parent].res)
			break;
		heap_swap(v, parent);
		v = parent;
	}
}

void pop(void) {
	heap_swap(0, --heap_len);
	push_down(0);
}

void update_res(int x, int y, int new_value) {
	int pos = get(heap_pos, x, y);
	set(res, x, y, new_value);
	heap[pos].res = new_value;
	push_up(pos);
}

int get_length(int x1, int y1, int x2, int y2) {
	return abs(get(field, x1, y1) - get(field, x2, y2));
}

void try_to_go(int x1, int y1, int x2, int y2) {
	int temp;
	if (x2 < 0 || x2 == n || y2 < 0 || y2 == m)
		return;
	temp = get(res, x1, y1) + get_length(x1, y1, x2, y2);
	if (temp < get(res, x2, y2))
		update_res(x2, y2, temp);
}

int main(void) {
	int i, j, x, y;
	scanf("%d%d%d%d%d%d", &n, &m, &n0, &m0, &n1, &m1);
	field = (int *)malloc(n * m * sizeof(int));
	heap_pos = (int *)malloc(n * m * sizeof(int));
	res = (int *)malloc(n * m * sizeof(int));
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			scanf("%d", field + i*m + j);
			if (i != n0 || j != m0) {
				heap[heap_len].x = i;
				heap[heap_len].y = j;
				heap[heap_len].res = INF;
				set(res, i, j, INF);
				set(heap_pos, i, j, heap_len++);
			}
		}
	heap[0].x = n0; heap[0].y = m0; heap[0].res = 0;
	set(res, n0, m0, 0); set(heap_pos, n0, m0, 0);

	while (heap_len) {
		x = heap[0].x; y = heap[0].y; pop();
		try_to_go(x, y, x - 1, y);
		try_to_go(x, y, x + 1, y);
		try_to_go(x, y, x, y - 1);
		try_to_go(x, y, x, y + 1);
	}

	printf("%d\n", get(res, n1, m1));
	free(field);
	free(heap_pos);
	free(res);
	return 0;
}