#ifndef MYFIND_H
#define MYFIND_H

#ifndef _DEFAULT_SOURCE
#    define _DEFAULT_SOURCE
#endif /* ! _DEFAULT_SOURCE */

/*** Includes ***/
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/*** Definitions ***/
#define PATH_LEN 2

/*** Structures ***/
enum
{
    P,
    H,
    L,
    d
};

typedef enum
{
    TEST,
    ACTION,
    OPERATOR
} a_type;

struct actions
{
    a_type type;
    char *name;
    bool (*func)(char *path, char **args, int otp, int *err);
    char **args;
};

struct node
{
    struct actions *action;
    struct node *left;
    struct node *right;
};

/*** Functions ***/
//* Main
int list_files(struct node *root, char *dir_path, int otp, int rec);

//% Parsing
struct actions **lexer(int argc, char **argv, int start);
struct node *parser(struct actions **expr);

//$ Solver
struct node *add_implicit_action(struct node *root);
bool evalAST(struct node *root, char *path, int opt, int *err);

//= Actions
bool action_print(char *path, char **args, int otp, int *err);
bool action_name(char *path, char **args, int otp, int *err);
bool action_type(char *path, char **args, int otp, int *err);
bool action_newer(char *path, char **arg, int opt, int *err);
bool action_perm(char *path, char **args, int otp, int *err);
bool action_user(char *path, char **args, int otp, int *err);
bool action_group(char *path, char **args, int otp, int *err);

//! Errors
void err_stop(char *arg);
void err_continue(char *arg);
void set_err(char *arg, int *err);

//? Utils
bool has_dir_slash(char *path);
bool is_actions(char *arg);
bool is_op(char *arg);
void print_ast(struct node *root, int level);
void free_ast(struct node *root);

#endif /* ! MYFIND_H */
