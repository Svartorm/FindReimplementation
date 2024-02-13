#include "../myfind.h"

struct actions *is_operator(char *str)
{
    if (strcmp(str, "-o") != 0 && strcmp(str, "-a") != 0
        && strcmp(str, "!") != 0 && strcmp(str, "(") != 0
        && strcmp(str, ")") != 0)
        return NULL;

    struct actions *action = malloc(sizeof(struct actions));
    action->type = OPERATOR;
    action->name = str;
    return action;
}

struct actions *is_test(char *str)
{
    if (strcmp(str, "-name") != 0 && strcmp(str, "-type") != 0
        && strcmp(str, "-newer") != 0 && strcmp(str, "-perm") != 0
        && strcmp(str, "-user") != 0 && strcmp(str, "-group") != 0)
        return NULL;

    struct actions *action = malloc(sizeof(struct actions));
    action->type = TEST;
    action->name = str;
    action->func = strcmp(str, "-name") == 0 ? &action_name
        : strcmp(str, "-type") == 0          ? &action_type
        : strcmp(str, "-newer") == 0         ? &action_newer
        : strcmp(str, "-perm") == 0          ? &action_perm
        : strcmp(str, "-user") == 0          ? &action_user
        : strcmp(str, "-group") == 0         ? &action_group
                                             : NULL;
    action->args = malloc(sizeof(char *) * 2);
    action->args[1] = NULL;
    return action;
}

struct actions *is_action(char *str)
{
    struct actions *action;

    if (strcmp(str, "-print") == 0)
    {
        action = malloc(sizeof(struct actions));
        action->func = &action_print;
    }
    /* For now we ignore exec and execdir
    else if (strcmp(str, "-exec") == 0)
    {
        //TODO
    }
    else if (strcmp(str, "-execdir") == 0)
    {
        //TODO
    }
    */
    else
        return NULL;

    action->type = ACTION;
    action->name = str;
    return action;
}

void add_action(int argc, struct actions **actions, struct actions *action,
                int *u)
{
    if (*u >= argc)
        actions = realloc(actions, sizeof(struct actions *) * (argc * 2));

    actions[*u] = action;
    *u += 1;
}

void add_and(int argc, struct actions **actions, int *u)
{
    struct actions *action = malloc(sizeof(struct actions));
    action->type = OPERATOR;
    action->name = "-a";
    add_action(argc, actions, action, u);
}

void add_print(int argc, struct actions **actions, int *u)
{
    struct actions *action = malloc(sizeof(struct actions));
    action->type = ACTION;
    action->name = "-print";
    action->func = &action_print;
    add_action(argc, actions, action, u);
}

struct actions **lexer(int argc, char **argv, int start)
{
    struct actions **actions = malloc(sizeof(struct actions *) * argc * 2);

    int i = start;
    int u = 0;
    while (i < argc)
    {
        //= Case if the argument is an operator
        struct actions *new = is_operator(argv[i]);
        if (new != NULL)
        {
            //* Check if we need to add a implicit 'and' operator
            if (strcmp(new->name, "!") == 0 || strcmp(new->name, "(") == 0)
                if (u > 0
                    && (actions[u - 1]->type != OPERATOR
                        || strcmp(actions[u - 1]->name, ")") == 0))
                    add_and(argc, actions, &u);

            //* Add the operator
            add_action(argc, actions, new, &u);
            i++;
            continue;
        }

        //= Case if the argument is a test
        new = is_test(argv[i]);
        if (new != NULL)
        {
            //* Check if we need to add a implicit 'and' operator
            if (u > 0
                && (actions[u - 1]->type != OPERATOR
                    || strcmp(actions[u - 1]->name, ")") == 0))
                add_and(argc, actions, &u);

            //* Add the test
            add_action(argc, actions, new, &u);
            i++;
            if (i < argc)
            {
                new->args[0] = argv[i];
                i++;
            }
            else
                err_stop("missing argument");
            continue;
        }

        //= Case if the argument is an action
        new = is_action(argv[i]);
        if (new != NULL)
        {
            //* Check if we need to add a implicit 'and' operator
            if (u > 0
                && (actions[u - 1]->type != OPERATOR
                    || strcmp(actions[u - 1]->name, ")") == 0))
                add_and(argc, actions, &u);

            //* Add the action
            add_action(argc, actions, new, &u);
            i++;
            continue;
        }

        //= Unknown argument
        err_stop("myfind: invalid argument");
    }

    actions[u] = NULL;
    return actions;
}
