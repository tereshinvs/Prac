//      main.c


int swaps = 0, comparings = 0;


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heapsort.h"
#include "shellsort.h"
#include "timestamp.h"


int cmp_type = -1;
static void (*sort)(long long *, int, int) = NULL;


static void init() {
	#ifdef DECREASING
	cmp_type = 1;
	#endif
	#ifdef INCREASING
	cmp_type = 0;
	#endif
	
	if (cmp_type == -1) {
		printf("Error sorting type. Recompile with -D INCREASING or DECREASING\n");
		exit(0);
	}

	#ifdef SHELL
	sort = shellsort;
	#endif
	#ifdef HEAP
	sort = heapsort;
	#endif
	
	if (!sort) {
		printf("Error sorting method. Recompile with -D SHELL or HEAP\n");
		exit(0);
	}

	printf("Sort method: %s\n", sort == heapsort ? "HEAPSORT" : "SHELLSORT");
	printf("Sort type: %s\n", cmp_type ? "DECREASING" : "INCREASING");
}


void alr_inc(long long *data, int len) {
	for (int i = 0; i < len; i++)
		data[i] = i;
}


void alr_dec(long long *data, int len) {
	for (int i = 0; i < len; i++)
		data[i] = len - i;
}


void rand_gen(long long *data, int len) {
	srand(time(NULL));
	for (int i = 0; i < len; i++)
		data[i] = rand() * rand() * rand() * rand();
}


typedef void (*gen)(long long *, int);
gen array_generator[] = { alr_inc, alr_dec, rand_gen, rand_gen }; // List of array generators
int arr_gen_k = 4; // array_generator length
int arr_len[] = { 10, 100, 1000, 100000 }; // List of needed arrat lengths
int arr_len_k = 4; // arr_len length


int main(int argc, char **argv) {
	init();
	
	for (int gen_j = 0; gen_j < arr_gen_k; gen_j++)
		for (int len_j = 0; len_j < arr_len_k; len_j++) {

			int len = arr_len[len_j];
			long long *data = (long long *)malloc(sizeof(long long) * len);
	
			printf("Array params: len = %7d", len);
			
			if (array_generator[gen_j] == alr_inc)
				printf("%12s\t", "increasing");
			if (array_generator[gen_j] == alr_dec)
				printf("%12s\t", "decreasing");
			if (array_generator[gen_j] == rand_gen)
				printf("%12s\t", "random");

			array_generator[gen_j](data, len);

			unsigned long long time_before = timestamp();
			sort(data, len, cmp_type);
			unsigned long long time_after = timestamp();

			printf("Comparings: %10d\t swaps: %10d\t time: %15lld\n", comparings, swaps, time_after - time_before);
			
			free(data);
		}

	return 0;
}

