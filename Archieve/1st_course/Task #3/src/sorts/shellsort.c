//      shellsort.c
//      


#include "shellsort.h"
#include "compare.h"
#include "swap.h"



extern int swaps, comparings;


extern void shellsort(long long *data, int len, int cmp_type) {
	swaps = comparings = 0;

	int (*cmp)(long long, long long) = cmp_type ? greater : less;

	for (int gap = len / 2; gap > 0; gap = gap / 2) // Gap is distanse between elements, which we compare
		for (int i = gap; i < len; i++) {
			int x = data[i], j;
			for (j = i - gap; j >= 0 && cmp(x, data[j]); j -= gap) {
				data[j + gap] = data[j]; // Shifting elements to the right, while current x is lower
				swaps++;
			}
			data[j + gap] = x;
			swaps++;
		}
}

