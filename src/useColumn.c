#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include "useColumn.h"
#include "common.h"
#include "useLong.h"
#include "linkedList.h"
#include "display.h"
#include "types.h"

void columnDisplay(linkedList l, int *ls, int *lm, FileInfo *filesInfo, int index, int *ts, int *tm, int cl, int ln)
{
    int x;
    File t;

    for (int i = 0; i < ln; i++)
    {
        for (int j = 0; j < cl; j++)
        {
            x = j * ln + i;
            if (x < l->count)
            {
                // if options -s, -p, -m, -u or -g is set
                t = (File)lGet(l, x);
                if (filesInfo)
                {
                    printFormattedValue(filesInfo[x].bfr[index], tm[j] - 1, 1);
                }
                display(t->name, &t->st.st_mode, False);
                x = lm[j] - ls[x];
                for (int k = 0; k < x; k++)
                { // the +1 is for the last space between columns.
                    printf(" ");
                }
                if (j != cl - 1)
                { // don't add space at the last column
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
}

void columnMain(linkedList l, FileInfo *filesInfo, int index)
{
    struct winsize w;
    int cl = l->count, ln = 1, winsiz, ok = 0;
    unsigned long int cnt;
    int *ls;        // linkedList sizes
    int *lm;        // linkedList max sizes
    int *ts = NULL; // array sizes of "tb"
    int *tm = NULL; // array max sizes of "tb"
    int x, y = 0;
    Iterator it;
    File file;
    int k;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    winsiz = w.ws_col;

    ls = (int *)memAlloc(sizeof(int) * l->count);
    lm = (int *)memAlloc(sizeof(int) * l->count);
    if (filesInfo)
    {
        ts = (int *)memAlloc(sizeof(int) * l->count);
        tm = (int *)memAlloc(sizeof(int) * l->count);
    }
    // length of each name.
    it = lAt(l, LFIRST);
    int nb_spaces;
    for (int i = 0; i < l->count; i++)
    {
        file = (File)it->data;
        ls[i] = strWidth(file->name);
        nb_spaces = countSpaces(file->name);
        if (nb_spaces)
        {
            if (Nparams & NB)
            {
                ls[i] += nb_spaces;
            }
            if (Nparams & NQ)
            {
                ls[i] += 2;
            }
        }
        if ((Nparams & NI) && (S_ISDIR(file->st.st_mode) ||
                           S_ISLNK(file->st.st_mode) ||
                           S_ISSOCK(file->st.st_mode) ||
                           S_ISFIFO(file->st.st_mode) ||
                           (S_IEXEC & file->st.st_mode)))
        {
            ls[i] += 1;
        }
        if (filesInfo)
        {
            ts[i] = strlen(filesInfo[i].bfr[index]) + 1; // +1 for space between "tb" and "l"
        }
        lInc(&it);
    }
    while (!ok)
    {
        cnt = 0;
        for (int i = 0; i < cl; i++)
        { // for each column "i", calculates the maximum of that column
            x = ls[i * ln];
            if (filesInfo)
            {
                y = ts[i * ln];
            }
            for (int j = 1; j < ln; j++)
            {
                k = i * ln + j;
                if (k < l->count)
                {
                    if (ls[k] > x)
                    {
                        x = ls[k];
                    }
                    if (filesInfo && (ts[k] > y))
                    {
                        y = ts[k];
                    }
                }
            }
            if (filesInfo)
            {
                cnt += x + y;
                tm[i] = y;
            }
            else
            {
                cnt += x;
            }
            lm[i] = x;
        }
        cnt += 2 * (cl - 1); // space between columns
        if (cnt <= winsiz)
        {
            ok = 1;
        }
        else
        {
            cl--;
            if (l->count % cl)
            {
                ln = l->count / cl + 1;
            }
            else
            {
                ln = l->count / cl;
            }
        }
    }
    columnDisplay(l, ls, lm, filesInfo, index, ts, tm, cl, ln);
    free(ls);
    free(ts);
    free(lm);
    free(tm);
}
