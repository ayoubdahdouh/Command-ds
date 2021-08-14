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

void tree_main(linklist l, int index, format_tree_t *tree)
{
    int last;
    int psiz;
    int if_a;
    lftype t;
    int j = 0;
    int n = l->count;

    if (LF_opt.tdeep && tree->level >= LF_opt.tdeep)
    {
        return;
    }

    if_a = LF_opt.a ? 3 : 1;
    // for files.
    while (j < index)
    {
        t = (lftype)(lat(l, LFIRST))->data;
        last = (j == n - if_a) ? 1 : 0;
        tree_display(tree, last);
        lfprint(t->name, &t->st.st_mode, true, true);
        free(t->name);
        free(t);
        ldel(l, LFIRST);
        ++j;
    }
    // for folders.
    psiz = LF_pathsiz;
    while (j < n)
    {
        t = (lftype)(lat(l, LFIRST))->data;
        if (strcmp(t->name, ".") && strcmp(t->name, ".."))
        {
            LF_pathsiz = psiz;
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
            tree->level++;
            // initial "path" and "pathsiz"
            strcpy(&LF_path[LF_pathsiz], t->name);
            LF_pathsiz = strlen(LF_path);
            // add slash to path
            LF_path[LF_pathsiz] = '/';
            LF_pathsiz++;
            LF_path[LF_pathsiz] = 0;
            core(tree);
            tree->level--;
        }
        free(t->name);
        free(t);
        ldel(l, LFIRST);
        ++j;
    }
}