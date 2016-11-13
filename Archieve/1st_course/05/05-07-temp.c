#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define INF INT_MAX/2

typedef int lnumber;

lnumber *field, *res, *heap_pos, n, m, n1, m1, curres, ttemp;
lnumber heap[1100000][2];

#define set(place, x, y, value) *(place + x*m + y) = value
#define get(place, x, y) *(place + x*m + y)
#define get_length(x1, y1, x2, y2) abs((lnumber)get(field, x1, y1) - (lnumber)get(field, x2, y2))

#define try_to_go(x1, y1, x2, y2, len)\
	ttemp = curres + get_length(x1, y1, x2, y2);\
	if (get(res, x2, y2) > ttemp) {\
		set(res, x2, y2, ttemp);\
		up(get(heap_pos, x2, y2), len);\
	}

/*void set(lnumber *place, lnumber x, lnumber y, lnumber value) {
	*(place + x*m + y) = value;
}

lnumber get(lnumber *place, lnumber x, lnumber y) {
	return *(place + x*m + y);
}   */

void heap_swap(lnumber t1, lnumber t2) {
	lnumber temp;
	set(heap_pos, heap[t1][0], heap[t1][1], t2);
	set(heap_pos, heap[t2][0], heap[t2][1], t1);
	temp = heap[t1][0]; heap[t1][0] = heap[t2][0]; heap[t2][0] = temp;
	temp = heap[t1][1]; heap[t1][1] = heap[t2][1]; heap[t2][1] = temp;
}

void heapify(lnumber v, lnumber len) {
	lnumber temp, left, right;
heapify:
	temp = get(res, heap[v][0], heap[v][1]), left, right;
	if ((v << 1) + 1 >= len)
		return;
	left = get(res, heap[(v << 1) + 1][0], heap[(v << 1) + 1][1]);
	if ((v << 1) + 2 >= len) {
		if (temp > left) {
			heap_swap(v, (v << 1) + 1);
			v = (v << 1) + 1;
			goto heapify;
			//heapify(v*2 + 1, len);
			}
		return;
	}
	right = get(res, heap[(v << 1) + 2][0], heap[(v << 1) + 2][1]);
	if (left < right) {
		if (temp > left) {
			heap_swap(v, (v << 1) + 1);
			v = (v << 1) + 1;
			goto heapify;
			//heapify(v*2 + 1, len);
			}
	} else {
		if (temp > right) {
			heap_swap(v, (v << 1) + 2);
			v = (v << 1) + 2;
			goto heapify;//heapify(v*2 + 2, len);
		}
	}
}

void up(lnumber v, lnumber len) {
	while (v > 0 && get(res, heap[v][0], heap[v][1]) < get(res, heap[(v - 1) >> 1][0], heap[(v - 1) >> 1][1])) {
		heap_swap((v - 1) >> 1, v);                                               	
		v = (v - 1) >> 1;
		if (get(res, heap[v][0], heap[v][1]) > get(res, heap[(v << 1) + 1][0], heap[(v << 1) + 1][1]) ||
			get(res, heap[v][0], heap[v][1]) > get(res, heap[(v << 1) + 2][0], heap[(v << 1) + 2][1]))
				break;
	}
//	heapify(v, len);
}

void pop(lnumber len) {
	heap_swap(0, len - 1);
	if (len > 1)
		heapify(0, len - 1);
}
/*
lnumber get_length(lnumber x1, lnumber y1, lnumber x2, lnumber y2) {
	return ;
}*/
/*
void update_res(lnumber x, lnumber y, lnumber new_value, lnumber len) {
	set(res, x, y, new_value);
	up(get(heap_pos, x, y), len);
}*/
/*
inline void try_to_go(lnumber x1, lnumber y1, lnumber x2, lnumber y2, lnumber len) {
	lnumber temp = curres + get_length(x1, y1, x2, y2);
	if (get(res, x2, y2) > temp) {
		set(res, x2, y2, temp);
		up(get(heap_pos, x2, y2), len);
	}
	//	update_res(x2, y2, curres + get_length(x1, y1, x2, y2), len);
}                                                           */

void out_heap(lnumber len) {
	lnumber i;
	printf("len = %d\n", len);
	for (i = 0; i < len; i++)
		printf("(%d %d %d) ", heap[i][0], heap[i][1], get(res, heap[i][0], heap[i][1]));
	printf("\n");
}

int main(void) {
//	freopen("output.txt", "w", stdout);
	lnumber k = 1, len, n0, m0, i, j, x, y;
	scanf("%d%d%d%d%d%d", &n, &m, &n0, &m0, &n1, &m1);
	field = (lnumber *)malloc(n * m * sizeof(lnumber));
	res = (lnumber *)malloc(n * m * sizeof(lnumber));
	heap_pos = (lnumber *)malloc(n * m * sizeof(lnumber));
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			scanf("%d", field + i*m + j);
			set(res, i, j, (i == n0 && j == m0) ? 0 : INF);
			if (i == n0 && j == m0) {
				heap[0][0] = i;
				heap[0][1] = j;
				set(heap_pos, i, j, 0);
			} else {
				heap[k][0] = i;
				heap[k][1] = j;
				set(heap_pos, i, j, k++);
			}
		}
//	printf("reading\n");
/*	int alr[n][m];
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			alr[i][j] = 0;*/
	for (len = n * m; len > 0;) {
//		out_heap(len);
//		printf("%llu\n", len);
		x = heap[0][0]; y = heap[0][1];// pop(len--);// alr[x][y]++;
		if (x == n1 && y == m1)
			break;
		heap_swap(0, len - 1);
		if (len > 1)
			heapify(0, len - 1);
		len--;
		curres = get(res, x, y);
/*		if (curres >= get(res, n1, m1))
			break;*/
		if (x > 0) try_to_go(x, y, (x - 1), y, len);
		if (x < n - 1) try_to_go(x, y, (x + 1), y, len);
		if (y > 0) try_to_go(x, y, x, (y - 1), len);
		if (y < m - 1) try_to_go(x, y, x, (y + 1), len);
	}
/*	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			printf("%d ", alr[i][j]);
		printf("\n");
	}     */
	printf("%d\n", get(res, n1, m1));
	free(res);
	free(field);
	free(heap_pos);
	fflush(stdout);
	return 0;
}