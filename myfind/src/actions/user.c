#include "../myfind.h"

bool action_user(char *path, char **args, int opt, int *err)
{
    struct stat path_stat;
    struct passwd *pwd;

    //? Get file stats.
    if (opt == L || opt == H)
        stat(path, &path_stat);
    else
        lstat(path, &path_stat);

    //? Get user name.
    pwd = getpwuid(path_stat.st_uid);
    if (pwd == NULL)
    {
        set_err("user: cannot get user name", err);
        return false;
    }

    //? Compare user name.
    return strcmp(pwd->pw_name, args[0]) == 0;
}
