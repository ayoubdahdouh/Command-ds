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
    for (int i = 0; i < l->count; i++)
    {
        t = (lf_type)it->data;
        if (has_space(t->name) || LFopt.nl->q)
        { // if name has space add +2, for the ""
            ls[i] = strlen(t->name) + 2;
        }
        else
        {
            ls[i] = strlen(t->name);
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
        if (LFopt.three)
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
            if (LFopt.zero)
            {
                printf(", ");
            }
            else if (LFopt.one)
            {
                printf("\n");
            }
            else if (LFopt.two)
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