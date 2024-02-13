#include "myfind.h"

int main(int argc, char *argv[])
{
    int p_len = PATH_LEN;
    char **path = malloc(sizeof(char *) * p_len);
    path[0] = ".";
    path[1] = NULL;

    /*
    ** Options:
    ** -H: Follow symbolic links on the command line, but don't follow symbolic
    *links
    **     encountered in the tree traversal.
    ** -L: Follow symbolic links encountered in the tree traversal.
    ** -P: Never follow symbolic links. This is the default behaviour.
    ** -d: Visit directories in post-order (after their content). This is the
    *default behaviour.
    */
    int opt = P;

    int i = 1;
    while (i < argc && argv[i][0] == '-')
    {
        if (strcmp(argv[i], "-H") == 0)
            opt = H;
        else if (strcmp(argv[i], "-L") == 0)
            opt = L;
        else if (strcmp(argv[i], "-P") == 0)
            opt = P;
        else if (strcmp(argv[i], "-d") == 0)
            opt = d;
        else
        {
            if (is_actions(argv[i]))
                break;
            else
                err_stop("myfind: invalid option");
        }
        i++;
    }

    //? If a path is specified, use it instead of the current directory.
    int u = 0;
    while (i < argc && !is_actions(argv[i]) && !is_op(argv[i]))
    {
        if (u >= p_len - 1)
        {
            p_len *= 2;
            path = realloc(path, sizeof(char *) * p_len);
        }
        path[u++] = argv[i++];
    }
    path[u] = u == 0 ? path[u] : NULL;

    //? Build the AST from the remaining arguments.
    // TODO
    struct actions **expr = lexer(argc, argv, i);
    struct node *root = parser(expr);

#ifdef DEBUG
    printf("Paths:\n");
    for (int i = 0; path[i]; i++)
        printf("%s\n", path[i]);
    printf("AST:\n");
    print_ast(root, 0);
#endif

    //? Add an implicit '-print' action if the AST does not have one.
    root = add_implicit_action(root);

#ifdef DEBUG
    printf("\nAST:\n");
    print_ast(root, 0);
#endif

    //? For each file/dir solve the AST.
    int e = 0; //$ Error flag.
    u = 0;
    while (path[u])
    {
        int res = list_files(root, path[u++], opt, 0);
        if (res == 1)
            e = 1;
    }

    //? Free the AST.
    free_ast(root);
    free(expr);
    free(path);

    return e;
}
