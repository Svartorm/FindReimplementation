#include "../myfind.h"

bool action_newer(char *path, char **arg, int opt, int *err)
{
    struct stat path_stat;
    struct stat arg_stat;

    if (opt == H || opt == L)
    {
        if (lstat(path, &path_stat) == -1)
        {
            set_err("newer: cannot stat file", err);
            return false;
        }

        if (lstat(arg[0], &arg_stat) == -1)
        {
            set_err("newer: cannot stat file", err);
            return false;
        }
    }
    else
    {
        if (stat(path, &path_stat) == -1)
        {
            set_err("newer: cannot stat file", err);
            return false;
        }

        if (stat(arg[0], &arg_stat) == -1)
        {
            set_err("newer: cannot stat file", err);
            return false;
        }
    }

    if (path_stat.st_mtime == arg_stat.st_mtime)
    {
        if (path_stat.st_mtim.tv_nsec == arg_stat.st_mtim.tv_nsec)
            return false;
        return path_stat.st_mtim.tv_nsec > arg_stat.st_mtim.tv_nsec;
    }

    return path_stat.st_mtime > arg_stat.st_mtime;
}
