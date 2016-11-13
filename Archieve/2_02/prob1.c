#include <fcntl.h>
#include <unistd.h>

double read_n(int fld, int n) {
	lseek(fld, n * sizeof(double), SEEK_SET);
	double res;
	read(fld, &res, sizeof(double));
	return res;
}

void write_n(int fld, int n, double val) {
	lseek(fld, n * sizeof(double), SEEK_SET);
	write(fld, &val, sizeof(double));
}

void swap(int fld, int left, int right) {
	double temp_left = read(fld, left), temp_right = read(fld, right);
	write_n(fld, left, temp_right);
	write_n(fld, right, temp_left);
}

void do_magic(int fld, double pivot) {
	int left = 0, right = lseek(fld, 0, SEEK_END) / sizeof(double) - 1;
	while (left <= right) {
		while (left <= right && read_n(fld, left) < pivot)
			left++:
		while (left <= right && read_n(fld, right) > pivot)
			right--;
		if (left <= right)
			swap(fld, left++, right--);
	}
}

int main(int argc, int *argv[]) {
	int fld = open(argv[1], O_RDWR);
	double pivot;
	sscanf(argv[2], "%lf", &pivot);

	do_magic(fld, pivot);

	close(fld);
	return 0;
}
