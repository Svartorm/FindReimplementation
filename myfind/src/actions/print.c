#include "../myfind.h"

/*
** This function is called when the -print action is encountered.
** It prints the path of the file.
** It must be called with args being NULL.
*/
bool action_print(char *path, char **args, int opt, int *err)
{
    if (args == NULL && opt == -1 && path == NULL && err == NULL)
    {
        printf("%s\n", path);
        return true;
    }

    printf("%s\n", path);
    return true;
}
