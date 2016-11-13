#include <stdio.h>

void print_read(void *x) {
	printf("R D 0x08%x\n", x);
}

void print_write(void *x) {
	printf("W D 0x08%x\n", x);
}

int main(int argc, char *argv[]) {
//****************************************************
// Хочется получить трассу запросов для:
	int *a, *b; // a == 1000, b == 2000

	for (int i = 0; i < 10000; i++) {
		a[i] = a[i] + b[i];
		print_read((void *)(1000 + i * sizeof(int)));
		print_read((void *)(2000 + i * sizeof(int)));
		print_write((void *)(1000 + i * sizeof(int)));
	}

	return 0;
}