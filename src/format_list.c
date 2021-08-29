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
    int winsiz;
    unsigned long int cnt;
    int *ls, *ts = NULL;
    iterator it;
    lf_type t;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    winsiz = w.ws_col;
    ls = (int *)lf_alloc(sizeof(int) * l->count);
    if (tb)
    {
        ts = (int *)lf_alloc(sizeof(int) * l->count);
    }
    // length of each name.
    it = lat(l, LFIRST);
    int nb_spaces;
    for (int i = 0; i < l->count; i++)
    {
        t = (lf_type)it->data;
        ls[i] = strlen(t->name);
        nb_spaces = has_space(t->name);
        if (nb_spaces)
        {
            if (LFopt.nl->b)
            {
                ls[i] += nb_spaces;
            }
            if (LFopt.nl->q || !LFopt.nl->b)
            {
               ls[i] += 2;
            }
        }
        else if (LFopt.nl->q)
        {
            ls[i] += 2;
        }
        if (S_ISDIR(t->st.st_mode) && LFopt.nl->s)
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
    int remain = winsiz;
    for (int i = 0; i < l->count; i++)
    {
        t = (lf_type)it->data;
        if (LFopt.zero)
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
            remain = winsiz - cnt;
            if (!LFopt.one)
            {
                printf("\n");
            }
        }
        display(t->name, &t->st.st_mode, false);
        if (i < l->count - 1)
        {
            if (LFopt.one)
            {
                printf("\n");
            }
            else if (LFopt.two)
            {
                printf(", ");
            }

            else if (LFopt.three)
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