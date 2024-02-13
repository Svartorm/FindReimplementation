#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(argc > 1 ? argv[1] : ".");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        fprintf(stderr, "simple_ls: bar: No such file or directory\n" );
        return 1;
    }

    while ((de = readdir(dr)) != NULL)
        if (de->d_name[0] != '.')
            printf("%s\n", de->d_name);

    closedir(dr);
    return 0;
}