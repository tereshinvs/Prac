int emptydir(char *dir_name) {
	DIR *cat = opendir(dirname);
	struct dirent *cur;
	while (cur = readdir(cat))
		if (!strcmp(".", cur->d_name) || !strcmp("..", cur->d_name))
			return 0;
	return 1;
}

int main(int argc, char *argv[]) {
	DIR *cat = opendir(argv[1]);
	struct dirent *cur;
	char filename[PATH_MAX];

	while (cur = readdir(cat)) {
		if (!strcmp(".", cur->d_name) || !strcmp("..", cur->d_name))
			continue;
		struct stat ab, ac;
		sprintf(filename, "%s/%s", argv[1], cur->d_name);
		if (stat(filename, &ab) != -1 && S_ISDIR(ab.st_mode)) {
			lstat(filename, &ac);
			if (S_ISDIR(ac)) {
				rmdir(filename);
				continue;
			}
		}
	}
	return 0;
}
