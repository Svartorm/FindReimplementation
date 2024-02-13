#include "../myfind.h"

bool action_type(char *path, char **args, int opt, int *err)
{
    if (opt == -1)
    {
        set_err("type: invalid options", err);
        return false;
    }

    struct stat path_stat;
    lstat(path, &path_stat);

    switch (args[0][0])
    {
    case 'b':
        return S_ISBLK(path_stat.st_mode);

    case 'c':
        return S_ISCHR(path_stat.st_mode);

    case 'd':
        return S_ISDIR(path_stat.st_mode);

    case 'f':
        return S_ISREG(path_stat.st_mode);

    case 'l':
        return S_ISLNK(path_stat.st_mode);

    case 'p':
        return S_ISFIFO(path_stat.st_mode);

    case 's':
        return S_ISSOCK(path_stat.st_mode);

    default:
        err_stop("type: invalid arguments");
        return false;
    }
}
