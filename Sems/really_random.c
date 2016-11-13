int rand(int l, int r) {
	int fd = open("/dev/urandom", O_RDONLY); // /dev/urandom generates pseudo-random if hasn't got really random numbers
	unsigned t;
	read(fd, &t, sizeof(unsigned));
	double x = (double)t / (1LL << 32);
	return l + (int)((r - l) * x);
}
