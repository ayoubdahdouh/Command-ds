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
#include "format_list.h"
#include "list.h"
#include "color.h"
#include "display.h"

void run(linklist a)
{
    char *nm;
    int i = 0;
    struct stat s;
    format_tree_t tree;
    lf_type t;
    linklist b;
    iterator it;
    int mularg = (a->count != 1);

    lf_init();
    memset(&tree, 0, LFTREESIZ);
    it = lat(a, LFIRST);
    b = lopen();
    while (it)
    {
        // initialise 'path' and 'pathsiz'
        LFpath[0] = 0;
        nm = (char *)it->data;
        if (!lf_stat(nm, &s))
        {
            printf("%s: \"%s\" %s\n", PROGRAM, nm, strerror(errno));
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                // init global variable "path" and "pathsiz"
                strcpy(LFpath, nm);
                LFpathsiz = strlen(nm);
                // if tree
                if (LFopt.t)
                { // if first time then allocate memory for "parent_has_next".
                    tree.level = 0;
                    if (!tree.parent_has_next)
                    {
                        tree.parent_has_next = (char *)lf_alloc(sizeof(char) * PATH_MAX);
                    }
                    if (LFpath[LFpathsiz - 1] == '/')
                    {
                        LFpathsiz--;
                        LFpath[LFpathsiz] = 0;
                    }
                    display(LFpath, &s.st_mode, true);
                }
                // add slash if doesn't have it.
                if (LFpath[LFpathsiz - 1] != '/')
                {
                    LFpath[LFpathsiz] = '/';
                    LFpathsiz++;
                    LFpath[LFpathsiz] = 0;
                }
                // if multiple arguments then print name of each argument.
                if (mularg)
                {
                    printf("%s:\n", LFpath);
                }
                // engine :)
                core(&tree);
            }
            else
            {
                lreset(b);
                t = (lf_type)lf_alloc(LFSIZ);
                t->st = s;
                t->name = nm;
                ladd(b, LFIRST, t);
                if (mularg)
                {
                    printf("%s:\n", nm);
                }
                if (LFopt.l)
                {
                    long_main(b);
                }
                else
                {
                    display(nm, &s.st_mode, true);
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

int stringcmp(char *s1, char *s2)
{
    int n, l1, l2, ok;
    char c1, c2;

    if (!s1 && !s2)
    {
        return 0;
    }
    else if (!s1)
    {
        return -1;
    }
    else if (!s2)
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
int sort_name(lf_type t1, lf_type t2)
{
    return stringcmp(t1->name, t2->name);
}
int sort_i(lf_type t1, lf_type t2)
{
    return (t1->st.st_ino < t2->st.st_ino) ? -1 : 1;
}
int sort_n(lf_type t1, lf_type t2)
{
    return (t1->st.st_nlink < t2->st.st_nlink) ? -1 : 1;
}
int sort_u(lf_type t1, lf_type t2)
{
    return (t1->st.st_uid < t2->st.st_uid) ? -1 : 1;
}
int sort_g(lf_type t1, lf_type t2)
{
    return (t1->st.st_gid < t2->st.st_gid) ? -1 : 1;
}
int sort_s(lf_type t1, lf_type t2)
{
    return (t1->st.st_size < t2->st.st_size) ? -1 : 1;
}
int sort_a(lf_type t1, lf_type t2)
{
    return (t1->st.st_atime < t2->st.st_atime) ? -1 : 1;
}
int sort_m(lf_type t1, lf_type t2)
{
    return (t1->st.st_mtime < t2->st.st_mtime) ? -1 : 1;
}
int sort_c(lf_type t1, lf_type t2)
{
    return (t1->st.st_ctime < t2->st.st_ctime) ? -1 : 1;
}
int sort_t(lf_type t1, lf_type t2)
{
    return ((t1->st.st_mode) < (t2->st.st_mode)) ? -1 : 1;
}

int sort_e(lf_type t1, lf_type t2)
{
    if (!t1 || !t2)
    {
        return 0;
    }
    return stringcmp(fileextension(t1->name), fileextension(t2->name));
}

void sort(linklist l)
{
    switch (LFopt.sc)
    {
    case 'i':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_i);
        break;

    case 'n':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_n);
        break;

    case 'u':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_u);
        break;

    case 'g':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_g);
        break;

    case 's':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_s);
        break;

    case 'a':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_a);
        break;

    case 'm':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_m);
        break;

    case 'c':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_c);
        break;

    case 't':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_t);
        break;

    case 'e':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_e);
        break;
    default:
        break;
    }
}
void core(format_tree_t *tree)
{
    DIR *d;
    struct dirent *f;
    struct stat s;
    linklist l;
    lf_type t;

    // keep value of "path" and "path_z"
    d = opendir(LFpath);
    if (!d)
    {
        strcpy(LFbuf, strerror(errno));
        if (LFopt.t)
        {
            tree_display(tree, 1);
            /**
             * 
             * Modify the error inst.
             * 
             * */
            printf("access denied: %s\n", LFbuf);

            LFbuf[0] = 0;
        }
        else
        {
            printf("%s: access denied to \"%s\": %s\n", PROGRAM, LFpath, LFbuf);
        }
        return;
    }
    l = lopen();
    while ((f = readdir(d)))
    {
        if (LFopt.a || (f->d_name[0] != '.'))
        {
            strcpy(&LFpath[LFpathsiz], f->d_name);
            if (!lf_stat(LFpath, &s))
            {
                printf("%s: %s: %s", PROGRAM, LFpath, strerror(errno));
                lf_quit();
            }
            else
            {
                /**
                 *  case where link is dir
                 *  allocate strlen(f->d_name) + 2 // +2 is for the "/"
                 **/
                t = (lf_type)lf_alloc(LFSIZ);
                t->st = s;
                t->name = (char *)lf_alloc(sizeof(char) * (strlen(f->d_name) + 1));
                strcpy(t->name, f->d_name);
                if ((S_ISDIR(s.st_mode) && LFopt.ml->d) ||
                    (S_ISBLK(s.st_mode) && LFopt.ml->b) ||
                    (S_ISCHR(s.st_mode) && LFopt.ml->c) ||
                    (S_ISFIFO(s.st_mode) && LFopt.ml->p) ||
                    (S_ISLNK(s.st_mode) && LFopt.ml->l) ||
                    (S_ISSOCK(s.st_mode) && LFopt.ml->s) ||
                    ((S_IREAD & s.st_mode) && LFopt.ml->r) ||
                    ((S_IWRITE & s.st_mode) && LFopt.ml->w) ||
                    ((S_IEXEC & s.st_mode) && LFopt.ml->x) ||
                    (S_ISREG(s.st_mode) && LFopt.ml->f))
                {
                    ladd(l, LLAST, t);
                }
            }
        }
    }
    closedir(d);
    if (lempty(l))
    {
        return;
    }
    if (LFopt.s)
    {
        sort(l);
    }
    else
    {
        // sort by name.
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))sort_name);
    }
    if (LFopt.t)
    { // format tree
        tree_main(l, tree);
    }
    else if (LFopt.l)
    { // format long
        long_main(l);
    }
    else if (LFopt.zero || LFopt.one || LFopt.two || LFopt.three)
    { // format long
        list_main(l, NULL);
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
            t = (lf_type)i->data;
            free(t->name);
            free(t);
            linc(&i);
        }
    }
    lclose(l);
}