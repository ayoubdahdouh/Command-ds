#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include "format_tree.h"
#include "common.h"
#include "lf.h"
#include "list.h"
#include "display.h"

void tree_display(format_tree_t *tree, bool last)
{
    for (int j = 0; j < tree->level; j++)
    {
        if (tree->parent_has_next[j] == '1')
        {
            printf("│  ");
        }
        else
        {
            printf("   ");
        }
    }
    if (last)
    {
        printf("└─ ");
    }
    else
    {
        printf("├─ ");
    }
}

void tree_main(linklist l, format_tree_t *tree)
{
    bool last;
    int psiz;
    int all;
    lf_type t;
    int i = 0, n;

    all = LFopt.a ? 2 : 0;
    psiz = LFpathsiz;
    n = l->count - all;

    while (i < n)
    {
        LFpathsiz = psiz;
        last = (i == n - 1) ? true : false;
        t = (lf_type)(lat(l, LFIRST))->data;
        if (S_ISDIR(t->st.st_mode))
        {
            if (strcmp(t->name, ".") && strcmp(t->name, ".."))
            {
                ++i;
                if (last)
                {
                    tree->parent_has_next[tree->level] = '0';
                }
                else
                {
                    tree->parent_has_next[tree->level] = '1';
                }
                tree_display(tree, last);
                display(t->name, &t->st.st_mode, true);
                if (!LFopt.td || tree->level + 1 < LFopt.td)
                {
                    tree->level++;
                    // initial "path" and "pathsiz"
                    strcpy(&LFpath[LFpathsiz], t->name);
                    LFpathsiz = strlen(LFpath);
                    // add slash to path
                    LFpath[LFpathsiz] = '/';
                    LFpathsiz++;
                    LFpath[LFpathsiz] = 0;
                    core(tree);
                    tree->level--;
                }
            }
        }
        else
        {
            ++i;
            tree_display(tree, last);
            display(t->name, &t->st.st_mode, true);
        }
        free(t->name);
        free(t);
        ldel(l, LFIRST);
    }
}