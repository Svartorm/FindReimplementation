#include "../myfind.h"

int list_files(struct node *root, char *dir_path, int opt, int rec)
{
    struct dirent *entry;
    DIR *dir = opendir(dir_path);

    int e = 0; //$ Error flag.

    if (dir == NULL)
    {
        closedir(dir);

        //? Check if the file exists.
        if (access(dir_path, F_OK) == -1)
        {
            set_err("myfind: No file or directory", &e);
            return e;
        }
        //?stat
        struct stat entry_stat;
        if (opt == L || (opt == H && rec == 0))
            stat(dir_path, &entry_stat);
        else
            lstat(dir_path, &entry_stat);

        evalAST(root, dir_path, opt, &e);

        return e;
    }

    if (opt != d)
        evalAST(root, dir_path, opt, &e);

    while ((entry = readdir(dir)) != NULL)
    {
        //? Vars
        struct stat entry_stat;
        char entry_path[1024];

        //? Get the path of the entry.
        snprintf(entry_path, sizeof(entry_path),
                 has_dir_slash(dir_path) ? "%s%s" : "%s/%s", dir_path,
                 entry->d_name);

        //? Get the stat of the entry.
        if (opt == L || (opt == H && rec == 0))
            stat(entry_path, &entry_stat);
        else
            lstat(entry_path, &entry_stat);

        //? Solve the AST for every entry.
        if (S_ISDIR(entry_stat.st_mode))
        {
            if (strcmp(entry->d_name, ".") != 0
                && strcmp(entry->d_name, "..") != 0)
            {
                int res = list_files(root, entry_path, opt, 1);
                if (res == 1)
                    e = 1;
            }
        }
        else
            evalAST(root, entry_path, opt, &e);
    }

    if (opt == d)
        evalAST(root, dir_path, opt, &e);

    closedir(dir);
    return e;
}
