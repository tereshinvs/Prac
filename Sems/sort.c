int a[N]; // 0x100000

for (int i = 0; i < N - 1; ++i)
	for (int j = 0; j < N - 1; ++j) {
		print_read(addr + 4 * j);
		print_read(addr + 4 * (j + 1));
		if (a[j] < a[j + 1]) {
			int t = a[j]; a[j] = a[i]; a[j + 1] = t;
			print_read(addr + j * 4);
			print_read(addr + (j + 1) * 4);
			print_write(addr + j * 4);
			print_write(addr + (j + 1) * 4);
		}
	}