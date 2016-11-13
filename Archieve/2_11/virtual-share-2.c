#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "r");
	while (fscanf(f1, "%x-%x r-x%c %x %d-%d %d", &start, &end, &type, &))
	return 0;
}