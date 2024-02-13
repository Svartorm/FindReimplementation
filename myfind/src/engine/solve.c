#include "../myfind.h"

bool has_action(struct node *root)
{
    if (root == NULL)
        return false;

    if (root->action->type == ACTION)
        return true;
    else
        return (root->left && root->left->action->type == ACTION)
            || (root->right && root->right->action->type == ACTION);
}

/*
** Add a print action if the root node does not have one action as a child.
** @param root The root node of the AST.
*/
struct node *add_implicit_action(struct node *root)
{
    if (root == NULL)
    {
        struct actions *print = malloc(sizeof(struct actions));
        print->type = ACTION;
        print->name = "-print";
        print->args = NULL;
        print->func = &action_print;

        struct node *pnode = malloc(sizeof(struct node));
        pnode->action = print;
        pnode->left = NULL;
        pnode->right = NULL;

        return pnode;
    }

    if (has_action(root))
        return root;

    /*
    ** New tree will be: tree -a -print
    */
    //? Create a new '-print' action node
    struct actions *print = malloc(sizeof(struct actions));
    print->type = ACTION;
    print->name = "-print";
    print->args = NULL;
    print->func = &action_print;

    struct node *pnode = malloc(sizeof(struct node));
    pnode->action = print;
    pnode->left = NULL;
    pnode->right = NULL;

    //? Create a new '-a' operator node
    struct actions *a = malloc(sizeof(struct actions));
    a->type = OPERATOR;
    a->name = "-a";

    struct node *anode = malloc(sizeof(struct node));
    anode->action = a;
    anode->left = root;
    anode->right = pnode;

    return anode;
}

bool evalAST(struct node *root, char *path, int opt, int *err)
{
    if (root == NULL)
        return true;

    if (root->action->type == OPERATOR)
    {
        if (strcmp(root->action->name, "-o") == 0)
            return evalAST(root->left, path, opt, err)
                || evalAST(root->right, path, opt, err);

        else if (strcmp(root->action->name, "-a") == 0)
            return evalAST(root->left, path, opt, err)
                && evalAST(root->right, path, opt, err);

        else
            err_stop("EvalAST: Invalid operator");
    }

    else if (root->action->type == TEST || root->action->type == ACTION)
        return root->action->func(path, root->action->args, opt, err);

    else
        err_stop("EvalAST: Invalid node type");

    return false; //? To remove the warning: is never reached
}
