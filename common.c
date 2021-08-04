#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include "common.h"
#include "list.h"

int has_space(const char *name)
{
    int i, tmp;

    tmp = strlen(name);
    for (i = 0; (i < tmp) && (name[i] != ' '); i++)
    {
    }
    return i != tmp;
}

void *lf_alloc(long int size)
{
    void *b = NULL;

    b = malloc(size);
    if (!b)
    {
        lf_error(errno, NULL, false);
    }
    return b;
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
int lf_link(const char *nm)
{
    int readlinksiz;
    readlinksiz = readlink(path, buf, PATH_MAX - 1);
    if (readlinksiz == -1)
    {
        lf_error(errno, NULL, false);
        return 0;
    }
    buf[readlinksiz] = 0;
    return 1;
}

// if failed, also print error in the buffer "buf"
int lf_stat(const char *nm, struct stat *s)
{
    if (lstat(nm, s) == -1)
    {
        lf_error(errno, NULL, false);
        return 0;
    }
    return 1;
}

void lf_error(int e, char *m, bool is_sys_err)
{
    char *s;
    if (is_sys_err)
    {
        s = strerror(e);
        if (s)
        {
            strcpy(buf, s);
        }
        else
        {
            buf[0] = 0;
        }
        if ((e == ENOMEM))
        {
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (e == ERR_INVALID_OPTION)
        {
            printf("%s: Invalid option \"%s\"\n", PROGRAM, m);
            lf_quit();
        }
        else if (e == ERR_COLORS_NOT_AVILABLE)
        {
            printf("%s : Warning: cannot use \"-c\" enavailable because the environment variable \"LS_COLORS\" is not defined.\n", PROGRAM);
            lf_quit();
        }
        else
        {
            strcpy(buf, m);
        }
    }
}

void lf_init()
{
    setbuf(stdout, NULL);
    // init PATH
    path = (char *)lf_alloc(sizeof(char) * PATH_MAX);
    // init BUF
    buf = (char *)lf_alloc(sizeof(char) * PATH_MAX);
    // init OPT
    memset(&opt, 0, OPTIONSIZ);
}

void lf_quit()
{
    free(path);
    free(buf);
    exit(EXIT_SUCCESS);
}

int is_absolute_path(const char *pth)
{
    if (pth)
    {
        return (path[0] == '/');
    }
    return 0;
}

char *lfext(char *pth)
{
    if (pth)
    {
        return NULL;
    }
    int n = strlen(pth) - 1, i = n, ok = 1;
    while (ok &&
           (i > 0) &&
           (pth[i] != '.'))
    {
        if (pth[i] == '/')
        {
            ok = 0;
        }
        else
        {
            --i;
        }
    }
    if (ok && i != n)
    {
        return &pth[i + 1];
    }
    return NULL;
}