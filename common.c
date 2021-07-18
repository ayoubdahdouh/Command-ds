#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include "common.h"

int has_space(const char *name)
{
    int i, tmp;

    tmp = strlen(name);
    for (i = 0; (i < tmp) && (name[i] != ' '); i++)
    {
    }
    return i != tmp;
}
void lf_error(int err, char *cause)
{
    switch (err)
    {
    case 1:
        break;
    case 2:
        printf("%s: cannot access '%s' : No such file or directory\n", PROGRAM, cause);
        break;
    case 3:
        printf("%s: Memory allocation failed.\n", PROGRAM);
        break;
    case 4:
        printf("%s: invalid option '%s'.\n", PROGRAM, cause);
        help();
        break;
    case 7:
        if (cause)
        {
            printf("%s: an internal error occurred (\"%s\", \"%s\").\n", PROGRAM, cause, path);
        }
        else
        {
            printf("%s: an internal error occurred.\n", PROGRAM);
        }
        break;
    default:
        break;
    }
    exit(EXIT_FAILURE);
}

void *lf_malloc(long int size)
{
    void *buf = NULL;

    buf = malloc(size);
    if (!buf)
    {
        lf_error(3, NULL);
    }
    return buf;
}

void help()
{
    printf("USAGE:\n");
    printf("     %s [OPTIONS] [DIR] ...\n\n", PROGRAM);
    printf("OPTIONS:\n");
    printf("    -a  show hidden files.\n");
    printf("    -d  show only directories.\n");
    printf("    -f  show only files.\n");
    printf("    -g  show groups of files.\n");
    printf("    -h  print help.\n");
    printf("    -l  show all information about the file (permissions, size,...).\n");
    printf("    -m  show modification time of each file.\n");
    printf("    -p  show permissions of files.\n");
    printf("    -r  used with -s, makes the size readable as 4K, 13M, 2G, etc.\n");
    printf("    -s  show sizes of files.\n");
    printf("    -t  show the contents of the subdirectories.\n");
    printf("    -u  show the owners of files.\n");
    printf("    -v  version of lf command.\n\n");
}

void version()
{
    printf("Version: %s %s.\n", PROGRAM, VERSION);
}

// places the contents of the symbolic link pathname in the buffer "buf" (global variable).
// if failed, also print error in the buffer "buf"
int lf_reaklink(const char *fname)
{
    int readlinksiz;
    readlinksiz = readlink(path, buf, PATH_MAX - 1);
    if (readlinksiz == -1)
    {
        lf_path_error(errno);
        return 0;
    }
    buf[readlinksiz] = 0;
    return 1;
}

// if failed, also print error in the buffer "buf"
int lf_stat(const char *fname, struct stat *s)
{
    if (lstat(fname, s) == -1)
    {
        lf_path_error(errno);
        return 0;
    }
    return 1;
}

int is_absolute_path(const char *path)
{
    return (path[0] == '/');
}

void lf_print_color(const char *fname, mode_t *mode)
{
    struct stat s;
    int fl = 0; // follow_link
    int lk = 0; // link
    char *color, *color2;
    int multcol = 0;

    if (!opt.l && (opt.s ^ opt.p ^ opt.m ^ opt.u ^ opt.g))
    { // if only one of the options s,p,u,g,m  is setted
        multcol = 1;
    }
    if (opt.t || opt.l || opt.p || opt.s || opt.u || opt.g || opt.m)
    {
        fl = 1;
    }
    // follow link only if it aren't multiple columns.
    fl = fl && !multcol;
    switch (*mode & S_IFMT)
    {
    case S_IFDIR:
        // if directory, blue colour
        color = BLUE;
        color2 = RST;
        // printf("%s%s/%s", BLUE, print_name(fname), RST);
        break;
    case S_IFLNK:
        if (fl)
        {
            lk = 1;
            strcpy(&path[pathsiz], fname);
            if (lf_reaklink(path))
            {
                if (!is_absolute_path(buf))
                {
                    strcpy(&path[pathsiz], buf);
                    strcpy(buf, path);
                }
                if (lf_stat(buf, &s))
                {
                    if (S_ISDIR(s.st_mode))
                    { // if link and readpath are directories, blue colour
                        color = CYAN;
                        color2 = BLUE;
                    }
                    else if (S_IXUSR & s.st_mode)
                    {
                        color = CYAN;
                        color2 = GREEN;
                    }
                    else if (S_IXUSR & *mode)
                    { // if link executable, green colour
                        color = CYAN;
                        color2 = RST;
                    }
                    strcpy(buf, &path[pathsiz]);
                }
                else
                {
                    color = CYAN;
                    color2 = RED;
                }
            }
        }
        else
        {
            color = CYAN;
            color2 = RST;
        }
        break;
    case S_IFREG:

        if (*mode & S_IXUSR)
        { // if executable, green colour
            color = GREEN;
            color2 = RST;
        }
        else
        {
            color = color2 = RST;
        }
        break;
    default:
        color = color2 = RST;
        break;
    }
    if (has_space(fname))
    {
        printf("%s\"%s\"%s", color, fname, RST);
    }
    else
    {
        printf("%s%s%s", color, fname, RST);
    }
    if (lk)
    {
        if (has_space(buf))
        {
            printf(" -> %s\"%s\"%s", color2, buf, RST);
        }
        else
        {
            printf(" -> %s%s%s", color2, buf, RST);
        }
    }
    if (fl)
    {
        printf("\n");
    }
}

void lf_print(char *fname, mode_t *mode)
{
    struct stat s;
    int fl = 0; // follow_link
    int lk = 0; // link
    int multcol = 0;

    if (!opt.l && (opt.s ^ opt.p ^ opt.m ^ opt.u ^ opt.g))
    { // if only one of the options s,p,u,g,m  is setted
        multcol = 1;
    }
    if (opt.c)
    {
        lf_print_color(fname, mode);
        return;
    }
    if (opt.t || opt.l || opt.p || opt.s || opt.u || opt.g || opt.m)
    {
        fl = 1;
    }
    fl = fl && !multcol;
    if (fl && ((*mode & S_IFMT) == S_IFLNK))
    {
        lk = 1;
        strcpy(&path[pathsiz], fname);
        if (lf_reaklink(path))
        {
            if (!is_absolute_path(buf))
            {
                strcpy(&path[pathsiz], buf);
                strcpy(buf, path);
            }
            if (lf_stat(buf, &s))
            {
                strcpy(buf, &path[pathsiz]);
            }
        }
    }
    if (has_space(fname))
    {
        if (S_ISDIR(*mode))
        {
            printf("\"%s\"/", fname);
        }
        else
        {
            printf("\"%s\"", fname);
        }
    }
    else
    {
        if (S_ISDIR(*mode))
        {
            printf("%s/", fname);
        }
        else
        {
            printf("%s", fname);
        }
    }
    if (lk)
    {
        if (has_space(buf))
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> \"%s\"/", buf);
            }
            else
            {
                printf(" -> \"%s\"", buf);
            }
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> %s/", buf);
            }
            else
            {
                printf(" -> %s", buf);
            }
        }
    }
    if (fl)
    {
        printf("\n");
    }
}

void lf_path_error(int err)
{
    switch (err)
    {
    case EACCES:
        buf = "Permission denied";
        break;
    case ENOENT:
        buf = "No such file or directory";
        break;
    case ENOTDIR:
        buf = "Not a directory";
        break;
    case ELOOP:
        buf = "Too many symbolic links encountered";
        break;
    case ENAMETOOLONG:
        buf = "File name too long";
        break;
    default:
        buf = "External error";
        break;
        break;
    }
}