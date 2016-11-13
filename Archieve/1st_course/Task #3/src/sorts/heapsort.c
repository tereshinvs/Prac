//      heapsort.c
//      


#include "heapsort.h"
#include "compare.h"
#include "swap.h"



extern int swaps, comparings;


static void heapify(long long *data, int len, int pos, int (*cmp)(long long, long long)) {
	//
	// Repair a heap in *data with length len from position pos using cmp as comparator
	//
	int j;
	for (int i = pos; i * 2 + 1 < len; i = j) {
		j = i * 2 + 1;
		if (j + 1 < len && cmp(data[j], data[j + 1]))
			j++;
		if (!cmp(data[i], data[j]))
			break;
		swap(data + i, data + j);
	}
}


extern void heapsort(long long *data, int len, int cmp_type) {
	swaps = comparings = 0;

	int (*cmp)(long long, long long) = cmp_type ? greater : less;

	for (int i = len / 2 - 1; i >= 0; i--)
		heapify(data, len, i, cmp);

	for (int i = len - 1; i >= 0; i--) {
		swap(data, data + i);
		heapify(data, i, 0, cmp);
	}
}
