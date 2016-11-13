#include <stdio.h>

#include "debug.h"

void test_3(char *name) {
	backtrace(name);
}

void test_2(char *name) {
	test_3(name);
}

void test_1(char *name) {
	int a = 5;
	test_2(name);
}

int main(int argc, char *argv[]) {
	printf("test.c is working...\n");
	test_1(argv[1]);
	return 0;
}
