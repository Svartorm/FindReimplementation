#include "../myfind.h"

bool action_group(char *path, char **args, int opt, int *err)
{
    struct stat path_stat;
    struct group *grp;

    //? Get file stats.
    if (opt == L || opt == H)
        stat(path, &path_stat);
    else
        lstat(path, &path_stat);

    //? Get group name.
    grp = getgrgid(path_stat.st_gid);
    if (grp == NULL)
    {
        set_err("cannot get group name", err);
        return false;
    }

    //? Compare group name.
    return strcmp(grp->gr_name, args[0]) == 0;
}
