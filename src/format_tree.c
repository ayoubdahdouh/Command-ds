#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "format_tree.h"
#include "common.h"
#include "lf.h"
#include "list.h"
#include "display.h"

void tree_display(_tree_info *tree, _bool islast)
{
    for (int j = 0; j < tree->level; j++)
    {
        if (tree->has_next[j] == '1')
        {
            printf("│  ");
        }
        else
        {
            printf("   ");
        }
    }
    if (islast)
    {
        printf("└─ ");
    }
    else
    {
        printf("├─ ");
    }
}

void tree_main(linklist l, _tree_info *tree)
{
    _bool islast;
    _file file;
    int i = 0, tmp_path_len = _path_len;
    int n = l->count - (_opt.ml->h ? 2 : 0);

    while (i < n)
    {
        islast = (i == n - 1) ? _true : _false;
        _path_len = tmp_path_len;
        file = (_file)(lat(l, LFIRST))->data;
        if (S_ISDIR(file->st.st_mode))
        {
            if (strcmp(file->name, ".") && strcmp(file->name, ".."))
            {
                ++i;
                if (islast)
                {
                    tree->has_next[tree->level] = '0';
                }
                else
                {
                    tree->has_next[tree->level] = '1';
                }
                tree_display(tree, islast);
                display(file->name, &file->st.st_mode, _true);
                if (tree->level + 1 < _opt.td)
                {
                    tree->level++;
                    strcpy(&_path[_path_len], file->name);
                    _path_len = strlen(_path);
                    _path[_path_len] = '/'; // add slash to path
                    _path_len++;
                    _path[_path_len] = 0;
                    core(tree);
                    tree->level--;
                }
            }
        }
        else
        {
            ++i;
            tree_display(tree, islast);
            display(file->name, &file->st.st_mode, _true);
        }
        free(file->name);
        free(file);
        ldel(l, LFIRST);
    }
}