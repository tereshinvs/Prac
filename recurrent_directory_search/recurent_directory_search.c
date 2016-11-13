#include <stdio.h>
#include <limits.h>
#include <sys/types.h>

void dfs(const char *dir) {
	DIR *cur_dir = opendir(dir);
	struct dirent *elem;
	while (elem = readdir(cur_dir)) {
		if (...) {}
		struct stat cur_stat;
		char cur_name[PATH_MAX];
		snprintf(cur_name, PATH_MAX, "%s/%s", dir, elem->d_name);
		lstat(cur_name, &cur_stat);
		if (S_ISDIR(cur_stat.st_mode)) {
			off_t offset = telldir(cur_dir);
			closedir(cur_dir);
			dfs(cur_name);
			cur_dir = opendir(dir);
			seekdir(cur_dir, offset);
		}
		if (S_LNK(cur_stat.st_mode))
			printf("%s\n", cur_name);
	}
	closedir(cur_dir);
}

int main(int argc, char *argv[]) {
	dfs("/");
	return 0;
}
