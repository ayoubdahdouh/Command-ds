#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "format_tree.h"
#include "common.h"
#include "lf.h"

void tree_display(format_tree_t *tree, int last)
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

void tree_main(format_tree_t *tree, char **tb, int tb_f, int tb_d, int tb_len)
{
    int last;
    struct stat s;
    int p_len;
    int if_a;

    if_a = opt.a ? 3 : 1;
    // for files.
    for (int i = 0; i < tb_f; i++)
    {
        strcpy(&path[pathsiz], tb[i]);
        // lf_stat(path, &s);
        if (lstat(path, &s) == -1)
        {
            printf("%s %s(access denied)%s\n", tb[i], RED, RST);
            return;
        }
        last = (i == tb_len - if_a) ? 1 : 0;
        tree_display(tree, last);
        lf_print(tb[i], &s.st_mode);
    }

    p_len = pathsiz;
    // for folders.
    for (int i = tb_f; i < tb_len; i++)
    {
        if (strcmp(tb[i], ".") && strcmp(tb[i], ".."))
        {
            pathsiz = p_len;
            strcpy(&path[pathsiz], tb[i]);
            if (lstat(path, &s) == -1)
            {
                printf("%s %s(access denied)%s\n", tb[i], RED, RST);
                return;
            }
            last = (i == tb_len - 1) ? 1 : 0;
            if (last)
            {
                tree->parent_has_next[tree->level] = '0';
            }
            else
            {
                tree->parent_has_next[tree->level] = '1';
            }
            tree_display(tree, last);
            lf_print(tb[i], &s.st_mode);
            tree->level++;
            pathsiz = strlen(path);
            path[pathsiz] = '/';
            pathsiz++;
            path[pathsiz] = '\0';
            core(tree);
            tree->level--;
        }
    }
}