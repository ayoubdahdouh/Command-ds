#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include "common.h"
#include "lf.h"
#include "format_column.h"
#include "format_long.h"
#include "format_tree.h"
#include "list.h"

void initial()
{
    setbuf(stdout, NULL);
    path = (char *)lf_malloc(sizeof(char) * PATH_MAX);
    buf = (char *)lf_malloc(sizeof(char) * PATH_MAX);
    // init all options to 0.
    memset(&opt, 0, LF_OPTIONS_Z);
}
void finish()
{
    free(buf);
    free(path);
}

void run(list arguments)
{
    char *tmp;
    struct stat s;
    format_tree_t tree;

    for (int i = 0; i < arguments->count; i++)
    {
        // initialise 'path' and 'path_len'
        path[0] = '\0';
        pathsiz = 0;
        tmp = (char *)l_at(arguments, i);
        lf_stat(tmp, &s);
        /**
         * 
         *  case where link is dir....
         * 
         */
        if (S_ISDIR(s.st_mode))
        {
            strcpy(path, tmp);
            pathsiz = strlen(tmp);
            if (opt.t)
            {
                // if first time then allocate memory for "parent_has_next".
                tree.level = 0;
                if (!tree.parent_has_next)
                {
                    tree.parent_has_next = (char *)lf_malloc(sizeof(char) * PATH_MAX);
                }
                if (path[pathsiz - 1] == '/')
                {
                    pathsiz--;
                    path[pathsiz] = '\0';
                }
                lf_print(path, &s.st_mode);
            }
            if (path[pathsiz - 1] != '/')
            {
                path[pathsiz] = '/';
                pathsiz++;
                path[pathsiz] = '\0';
            }
            if (arguments->count != 1)
            {
                printf("%s:\n", path);
            }
            core(&tree);
        }
        else
        {
            if (arguments->count != 1)
            {
                printf("%s:\n", tmp);
            }
            if (opt.l || opt.p || opt.s || opt.u || opt.g || opt.m)
            {
                long_main((char **)&tmp, 1, 1, 1);
            }
            else
            {
                lf_print(tmp, &s.st_mode);
                printf("\n");
            }
        }
        if (i != arguments->count - 1)
        {
            printf("\n");
        }
    }
}

int lf_count_dir_items(DIR *d)
{
    int i = 0;
    while (readdir(d))
    {
        i++;
    }
    rewinddir(d);
    return i;
}

// compare no case sensative
int str_cmp(char *s1, char *s2)
{
    int n, l1, l2, ok;
    char c1, c2;

    if (!s1 && !s2)
    {
        return 0;
    }
    if (!s1)
    {
        return -1;
    }
    if (!s2)
    {
        return 1;
    }
    l1 = strlen(s1);
    l2 = strlen(s2);
    if (l1 > l2)
    {
        n = l2;
    }
    else
    {
        n = l1;
    }
    ok = 0;
    for (int i = 0; !ok && i < n; i++)
    {
        // convert s1 to lower case
        if (s1[i] >= 'A' && s1[i] <= 'Z')
        {
            c1 = s1[i] + 32;
        }
        else
        {
            c1 = s1[i];
        }
        // convert s2 to lower case
        if (s2[i] >= 'A' && s2[i] <= 'Z')
        {
            c2 = s2[i] + 32;
        }
        else
        {
            c2 = s2[i];
        }
        if (c1 < c2)
        {
            ok = -1;
        }
        else if (c1 > c2)
        {
            ok = 1;
        }
    }
    if (!ok)
    {
        if (l1 > l2)
        {
            ok = 1;
        }
        else if (l1 < l2)
        {
            ok = -1;
        }
    }
    return ok;
}

void sort_tb(char **tb, int n)
{
    int min;
    char *tmp;
    for (int i = 0; i < n; i++)
    {
        min = i;
        for (int j = i; j < n; j++)
        {
            if (str_cmp(tb[j], tb[min]) < 0)
            {
                min = j;
            }
        }
        if (min != i)
        {
            tmp = tb[i];
            tb[i] = tb[min];
            tb[min] = tmp;
        }
    }
}

void core(format_tree_t *tree)
{
    DIR *d;
    struct dirent *f;
    struct stat s;
    int tb_len = 0;
    char **tb;
    int tb_d, tb_f = 0; // indexes of files and directories in "tb".
    char *tmp;

    // keep value of path and path_z
    if (!(d = opendir(path)))
    {
        if (opt.t)
        {
            tree_display(tree, 1);
            printf("%saccess denied%s\n", RED, RST);
        }
        else
        {
            printf("%s: access denied to \"%s\"\n", PROGRAM, path);
        }
        return;
    }
    tb_len = lf_count_dir_items(d);
    if (tb_len < 1)
    {
        return;
    }
    tb = lf_malloc(tb_len * sizeof(char *));
    tb_d = tb_len - 1;
    while ((f = readdir(d)))
    {
        if (opt.a || (f->d_name[0] != '.'))
        {
            strcpy(&path[pathsiz], f->d_name);
            lf_stat(path, &s);
            /**
             * 
             *  case where link is dir....
             * 
             */
            tmp = (char *)lf_malloc(sizeof(char) * (strlen(f->d_name) + 1));
            strcpy(tmp, f->d_name);
            if (S_ISDIR(s.st_mode))
            {
                if (opt.d)
                {
                    tb[tb_d] = tmp;
                    tb_d--;
                }
            }
            else if (opt.f)
            {
                tb[tb_f] = tmp;
                tb_f++;
            }
        }
    }

    // move empty cells to end of the array "tb".
    if (tb_f <= tb_d)
    {
        for (int i = 0; i < tb_len - tb_d - 1; i++)
        {
            tb[tb_f + i] = tb[tb_d + i + 1];
        }
        tb_len -= tb_d - tb_f + 1;
        tb_d = tb_f - 1;
    }
    sort_tb(tb, tb_f);
    sort_tb(&tb[tb_d + 1], tb_len - tb_d - 1);
    if (opt.t)
    { // format tree
        tree_main(tree, tb, tb_f, tb_d, tb_len);
    }
    else if (opt.l || opt.p || opt.s || opt.u || opt.g || opt.m)
    { // format long
        long_main(tb, tb_f, tb_d, tb_len);
    }
    else
    { // format column
        column_main(tb, NULL, tb_len);
    }
    // free array "tb"
    for (int i = 0; i < tb_len; i++)
    {
        free(tb[i]);
    }
    free(tb);
}