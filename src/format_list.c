#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list.h"
#include "format_list.h"
#include "common.h"
#include "display.h"

void list_main(linklist l, char **tb)
{
    struct winsize w;
    int window_size;
    unsigned long int cnt;
    int *ls, *ts = NULL;
    iterator it;
    _file file;
    int i = 0;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    window_size = w.ws_col;
    ls = (int *)_alloc(sizeof(int) * l->count);
    if (tb)
    {
        ts = (int *)_alloc(sizeof(int) * l->count);
    }
    // length of each name.
    int nb_spaces;
    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;
        ls[i] = strlen(file->name);
        nb_spaces = has_space(file->name);
        if (nb_spaces)
        {
            if (_opt.nl->b)
            {
                ls[i] += nb_spaces;
            }
            if (_opt.nl->q || !_opt.nl->b)
            {
                ls[i] += 2;
            }
        }
        else if (_opt.nl->q)
        {
            ls[i] += 2;
        }

        if (_opt.nl->i && (S_ISDIR(file->st.st_mode) ||
                           S_ISLNK(file->st.st_mode) ||
                           S_ISSOCK(file->st.st_mode) ||
                           S_ISFIFO(file->st.st_mode) ||
                           (S_IEXEC & file->st.st_mode)))
        {
            ls[i] += 1;
        }
        if (tb)
        {
            ts[i] = strlen(tb[i]) + 1; // +1 for space between "tb" and "l"
        }
        linc(&it);
    }
    it = lat(l, LFIRST);
    int remain = window_size;
    for (int i = 0; i < l->count; i++)
    {
        file = (_file)it->data;
        if (_opt._2)
        {
            cnt = ls[i] + 1;
        }
        else
        {
            cnt = ls[i] + 2;
        }
        if (tb)
        {
            cnt += strlen(tb[i]) + 1; // +1 for space between "tb" and "l"
        }
        if (cnt <= remain)
        {
            remain -= cnt;
        }
        else
        {
            remain = window_size - cnt;
            if (!_opt._1)
            {
                printf("\n");
            }
        }
        display(file->name, &file->st.st_mode, _false);
        if (i < l->count - 1)
        {
            if (_opt._1)
            {
                printf("\n");
            }
            else if (_opt._3)
            {
                printf(", ");
            }

            else if (_opt._4)
            {
                printf("; ");
            }
            else
            {
                printf(" ");
            }
        }
        else
        {
            printf("\n");
        }
        linc(&it);
    }
    free(ls);
    free(ts);
}