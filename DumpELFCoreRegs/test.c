int main(int argc, char *argv[]) {
	int a = 5;

	*((int *)a + 1) = 4;

	return 0;
}
