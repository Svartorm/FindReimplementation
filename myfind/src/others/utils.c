#include "../myfind.h"

bool has_dir_slash(char *path)
{
    int len = strlen(path);
    if (path[len - 1] == '/')
        return true;
    return false;
}

bool is_actions(char *arg)
{
    if (strcmp(arg, "-print") == 0 || strcmp(arg, "-name") == 0
        || strcmp(arg, "-type") == 0 || strcmp(arg, "-newer") == 0
        || strcmp(arg, "-perm") == 0 || strcmp(arg, "-user") == 0
        || strcmp(arg, "-group") == 0 || strcmp(arg, "-exec") == 0
        || strcmp(arg, "-execdir") == 0 || strcmp(arg, "-delete") == 0)
        return true;
    return false;
}

bool is_op(char *arg)
{
    if (strcmp(arg, "-a") == 0 || strcmp(arg, "-o") == 0
        || strcmp(arg, "(") == 0 || strcmp(arg, ")") == 0
        || strcmp(arg, "!") == 0)
        return true;
    return false;
}

void print_ast(struct node *root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");
    printf("%s\n", root->action->name);

    print_ast(root->left, level + 1);
    print_ast(root->right, level + 1);
}

void free_actions(struct actions *action)
{
    if (action == NULL)
        return;

    if (action->type == TEST
        || (action->type == ACTION && strcmp(action->name, "-print") != 0))
        free(action->args);
    free(action);
}

void free_ast(struct node *root)
{
    if (root == NULL)
        return;

    free_ast(root->left);
    free_ast(root->right);

    free_actions(root->action);
    free(root);
}
