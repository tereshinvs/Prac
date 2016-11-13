#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

long long
dir_size(const char *dir_path)
{
    char *sep = "/";
    if (dir_path[strlen(dir_path) - 1] == '/')
        sep = "";

    DIR *curdir;
    if (!(curdir = opendir(dir_path)))
        return 0;

    struct dirent *elem;
    long long res_size = 0;
    while ((elem = readdir(curdir))) {
        struct stat cur_stat;
        char s[PATH_MAX];
        snprintf(s, PATH_MAX, "%s%s%s", dir_path, sep, elem->d_name);
        if (lstat(s, &cur_stat) < 0)
            continue;
        if (S_ISREG(cur_stat.st_mode))
            res_size += cur_stat.st_size;
    }
    closedir(curdir);
    return res_size;
}

int
main(int argc, char *argv[])
{
    printf("%lld\n", dir_size(argv[1]));
    return 0;
}
