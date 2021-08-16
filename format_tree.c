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
    int last;
    int psiz;
    int if_all;
    lftype t;
    int j = 0;
    int n = l->count;

    if_all = LFopt.a ? 3 : 1;
    psiz = LFpathsiz;
    while (j < n)
    {
        t = (lftype)(lat(l, LFIRST))->data;
        if (S_ISDIR(t->st.st_mode))
        {
            if (strcmp(t->name, ".") && strcmp(t->name, ".."))
            {
                LFpathsiz = psiz;
                last = (j == n - 1) ? 1 : 0;
                if (last)
                {
                    tree->parent_has_next[tree->level] = '0';
                }
                else
                {
                    tree->parent_has_next[tree->level] = '1';
                }
                tree_display(tree, last);
                lfprint(t->name, &t->st.st_mode, true, true);
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
            last = (j == n - if_all) ? 1 : 0;
            tree_display(tree, last);
            lfprint(t->name, &t->st.st_mode, true, true);
        }

        free(t->name);
        free(t);
        ldel(l, LFIRST);
        ++j;
    }
}