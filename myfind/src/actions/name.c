#include <fnmatch.h>

#include "../myfind.h"

bool action_name(char *path, char **args, int opt, int *err)
{
    if (opt == -1)
        set_err("actions_name: invalid option", err);

    char *name = basename(path);

    if (fnmatch(args[0], name, 0) == 0)
        return true;
    return false;
}
