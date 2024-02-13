#include "../myfind.h"

bool action_perm(char *path, char **args, int opt, int *err)
{
    struct stat path_stat;
    mode_t mode;

    //? Get the information about the file.
    if (opt == H || opt == L)
    {
        if (lstat(path, &path_stat) == -1)
        {
            set_err("perm: cannot stat file", err);
            return false;
        }
    }
    else
    {
        if (stat(path, &path_stat) == -1)
        {
            set_err("perm: cannot stat file", err);
            return false;
        }
    }

    //? Get the mode of the file.
    sscanf((args[0][0] == '-' || args[0][0] == '/') ? args[0] + 1 : args[0],
           "%o", &mode);

    //? Compare the mode of the file with the mode given in the arguments.
    if (args[0][0] == '-') //% If all the permission bits are set for the file.
        return (path_stat.st_mode & mode) == mode;

    else if (args[0][0]
             == '/') //% If any of the permission bits are set for the file.
        return path_stat.st_mode & mode;

    //% If the permission bits are exactly the same for the file.
    return (path_stat.st_mode & 0777) == mode;
}
