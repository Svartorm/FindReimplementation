#include "../myfind.h"

void err_stop(char *arg)
{
    fprintf(stderr, "myfind: %s\n", arg);
    exit(1);
}

void err_continue(char *arg)
{
    fprintf(stderr, "myfind: %s\n", arg);
}

void set_err(char *arg, int *err)
{
    fprintf(stderr, "myfind: %s\n", arg);
    *err = 1;
}
