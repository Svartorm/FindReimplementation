#include "../myfind.h"

int get_len(struct actions **expr)
{
    int i = 0;
    while (expr[i])
        i++;
    return i;
}

int precedence(struct actions *op)
{
    if (strcmp(op->name, "-o") == 0)
        return 1;
    else if (strcmp(op->name, "-a") == 0)
        return 2;
    else if (strcmp(op->name, "!") == 0)
        return 3;
    else
        err_stop("Precedence: Invalid operator");
    return 0; //? To remove the warning: is never reached
}

int associativity(struct actions *op)
{
    if (strcmp(op->name, "-o") == 0)
        return 0;
    else if (strcmp(op->name, "-a") == 0)
        return 0;
    else if (strcmp(op->name, "!") == 0)
        return 1;
    else
        err_stop("Associativity: Invalid operator");
    return 0; //? To remove the warning: is never reached
}

struct node *parser(struct actions **expr)
{
    //? Get the length of the expression
    int len = get_len(expr);

    if (len == 0)
        return NULL;

    //? Init our stacks
    struct node **stack_node = malloc(sizeof(struct node *) * len);
    int n = -1;
    struct actions **stack_op = malloc(sizeof(struct actions *) * len);
    int o = -1;

    //* Parse the arguments and build the AST
    //*     using the Shunting-yard algorithm.

    for (int i = 0; i < len; i++)
    {
        if (expr[i]->type == OPERATOR)
        {
            struct actions *op = expr[i];
            //* Check for parenthesis
            if (strcmp(op->name, "(") == 0)
                stack_op[++o] = op;

            else if (strcmp(op->name, ")") == 0)
            {
                while (o >= 0 && strcmp(stack_op[o]->name, "(") != 0)
                {
                    //! If there is less than two operands in the stack,
                    //!     then there are too many operators.
                    if (n < 1)
                        err_stop("Parser: not enough operands");

                    //? Pop the operator
                    struct actions *op2 = stack_op[o--];

                    //? Pop the operands
                    struct node *right = stack_node[n--];
                    struct node *left = stack_node[n--];

                    //? Create a new node
                    struct node *new_node = malloc(sizeof(struct node));
                    new_node->action = op2;
                    new_node->left = left;
                    new_node->right = right;

                    //? Push the node to the stack
                    stack_node[++n] = new_node;
                }

                //! If the stack runs out without finding a left parenthesis,
                //!     then there are mismatched parentheses.
                if (o < 0)
                    err_stop("Parser: mismatched parentheses");

                //? Pop the left parenthesis and discard it
                free(stack_op[o--]);

                //? Free the right parenthesis
                free(op);
            }

            else //* Operator
            {
                while (o >= 0 && strcmp(stack_op[o]->name, "(") != 0
                       && (precedence(op) < precedence(stack_op[o])
                           || (precedence(op) == precedence(stack_op[o])
                               && associativity(op) == 0)))
                {
                    //! If there is less than two operands in the stack,
                    //!     then there are too many operators.
                    if (n < 1)
                        err_stop("Parser: not enough operands");

                    //? Pop the operator
                    struct actions *op2 = stack_op[o--];

                    //? Pop the operands
                    struct node *right = stack_node[n--];
                    struct node *left = stack_node[n--];

                    //? Create a new node
                    struct node *new_node = malloc(sizeof(struct node));
                    new_node->action = op2;
                    new_node->left = left;
                    new_node->right = right;

                    //? Push the node to the stack
                    stack_node[++n] = new_node;
                }
                //? Push the operator to the stack
                stack_op[++o] = op;
            }
        }

        else //* Test or action
        {
            //? Create a new node
            struct node *new_node = malloc(sizeof(struct node));
            new_node->action = expr[i];
            new_node->left = NULL;
            new_node->right = NULL;

            //? Push the node to the stack
            stack_node[++n] = new_node;
        }
    }

    //? Get all the remaining operators
    while (o >= 0)
    {
        //! If there is less than two operands in the stack,
        //!     then there are too many operators.
        if (n < 1)
            err_stop("Parser: not engough operands");

        //? Pop the operator
        struct actions *op = stack_op[o--];

        //? Pop the operands
        struct node *right = stack_node[n--];
        struct node *left = stack_node[n--];

        //? Create a new node
        struct node *new_node = malloc(sizeof(struct node));
        new_node->action = op;
        new_node->left = left;
        new_node->right = right;

        //? Push the node to the stack
        stack_node[++n] = new_node;
    }

    //? Return the root of the AST
    struct node *root = stack_node[n];

    //? Free the stacks
    free(stack_node);
    free(stack_op);

    return root;
}
