int skip_n_get(int fd, int n, char *dst) {
}

void process_file(int fd, int step) {
}

int main(int argc, char *argv[]) {
	int step = 2;
	for (int i = 1; i < argc; i++) {
		int fd = 0;
		if (argv[i][0] == '-') {
			if (strlen(argv[i]) != 1)
				sscanf(argc[i], "-%d", &step);
		} else {
			fd = open(argv[i], O_RDONLY);
		}
		process_file(fd, step);
		if (!fd)
			close(fd);
}
