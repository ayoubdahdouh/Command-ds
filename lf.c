#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <errno.h>
#include "common.h"
#include "lf.h"
#include "format_column.h"
#include "format_long.h"
#include "format_tree.h"
#include "list.h"
#include "color.h"
#include "display.h"

void run(linklist a)
{
    char *nm;
    int i = 0;
    struct stat s;
    format_tree_t tree;
    lftype t;
    linklist b;
    iterator it;
    int mularg = (a->count != 1);

    memset(&tree, 0, LFTREESIZ);
    it = lat(a, LFIRST);
    b = lopen();
    while (it)
    {
        // initialise 'path' and 'pathsiz'
        LF_path[0] = 0;
        nm = (char *)it->data;
        if (!lf_stat(nm, &s))
        {
            printf("%s: \"%s\" %s\n", PROGRAM, nm, LF_buf);
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                // init global variable "path" and "pathsiz"
                strcpy(LF_path, nm);
                LF_pathsiz = strlen(nm);
                // if tree
                if (LF_opt.t)
                { // if first time then allocate memory for "parent_has_next".
                    tree.level = 0;
                    if (!tree.parent_has_next)
                    {
                        tree.parent_has_next = (char *)lf_alloc(sizeof(char) * PATH_MAX);
                    }
                    if (LF_path[LF_pathsiz - 1] == '/')
                    {
                        LF_pathsiz--;
                        LF_path[LF_pathsiz] = 0;
                    }
                    lfprint(LF_path, &s.st_mode, true, false);
                }
                // add slash if doesn't have it.
                if (LF_path[LF_pathsiz - 1] != '/')
                {
                    LF_path[LF_pathsiz] = '/';
                    LF_pathsiz++;
                    LF_path[LF_pathsiz] = 0;
                }
                // if multiple arguments then print name of each argument.
                if (mularg)
                {
                    printf("%s:\n", LF_path);
                }
                // engine :)
                core(&tree);
            }
            else
            {
                lreset(b);
                t = (lftype)lf_alloc(LFSIZ);
                t->st = s;
                t->name = nm;
                ladd(b, LFIRST, t);
                if (mularg)
                {
                    printf("%s:\n", nm);
                }
                if (LF_opt.l || LF_opt.p || LF_opt.s || LF_opt.u || LF_opt.g || LF_opt.m)
                {
                    long_main(b);
                }
                else
                {
                    lfprint(nm, &s.st_mode, true, true);
                    printf("\n");
                }
            }
            if (i != a->count - 1)
            {
                printf("\n");
            }
        }
        linc(&it);
        ++i;
    }
    lclose(b);
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
int cmp(lftype t1, lftype t2)
{
    char *s1, *s2;
    int n, l1, l2, ok;
    char c1, c2;

    if (!t1 || !t2)
    {
        return 0;
    }
    s1 = t1->name;
    s2 = t2->name;
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

void core(format_tree_t *tree)
{
    DIR *d;
    struct dirent *f;
    struct stat s;
    linklist l;
    lftype t;
    int index;

    if (tree->level == 0)
    {
        int ok = 0;
        ++ok;
    }

    // keep value of "path" and "path_z"
    d = opendir(LF_path);
    if (!d)
    {
        strcpy(LF_buf, strerror(errno));
        if (LF_opt.t)
        {
            tree_display(tree, 1);
            /**
             * 
             * Modify the error inst.
             * 
             * */
            printf("\033[%saccess denied: %s\033[%s\n", getcolor(LF_lcolor, "rs", 0), LF_buf, getcolor(LF_lcolor, "rs", 0));
            LF_buf[0] = 0;
        }
        else
        {
            printf("%s: access denied to \"%s\": %s\n", PROGRAM, LF_path, LF_buf);
        }
        return;
    }
    l = lopen();
    index = 0;
    while ((f = readdir(d)))
    {
        if (LF_opt.a || (f->d_name[0] != '.'))
        {
            strcpy(&LF_path[LF_pathsiz], f->d_name);
            if (!lf_stat(LF_path, &s))
            {
                printf("%s: %s: %s", PROGRAM, LF_path, LF_buf);
                lf_quit();
            }
            else
            {
                /**
                 * 
                 *  case where link is dir
                 *  allocate strlen(f->d_name) + 2 // +2 is for the "/"
                 * 
                 **/
                t = (lftype)lf_alloc(LFSIZ);
                t->st = s;
                t->name = (char *)lf_alloc(sizeof(char) * (strlen(f->d_name) + 1));
                strcpy(t->name, f->d_name);
                if (S_ISDIR(s.st_mode))
                {
                    if (LF_opt.d)
                    {
                        ladd(l, LLAST, t);
                    }
                }
                else
                {
                    if (LF_opt.f)
                    {
                        ladd(l, LFIRST, t);
                        ++index;
                    }
                }
            }
        }
    }
    closedir(d);
    if (lempty(l))
    {
        return;
    }
    if (index > 0)
    { // if there's files
        lquicksort(l, LFIRST, index - 1, (int (*)(void *, void *))cmp);
    }
    if (index != l->count)
    { // if there's folders
        lquicksort(l, index, LLAST, (int (*)(void *, void *))cmp);
    }
    if (LF_opt.t)
    { // format tree
        tree_main(l, index, tree);
    }
    else if (LF_opt.l || LF_opt.p || LF_opt.s || LF_opt.u || LF_opt.g || LF_opt.m)
    { // format long
        long_main(l);
    }
    else
    { // format column
        column_main(l, NULL);
    }
    if (!lempty(l))
    {
        iterator i = lat(l, LFIRST);
        while (i)
        {
            t = (lftype)i->data;
            free(t->name);
            free(t);
            linc(&i);
        }
    }
    lclose(l);
}