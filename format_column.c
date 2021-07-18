#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include "format_column.h"
#include "common.h"
#include "format_long.h"

void column_display(char **tb, int *tb_max, int c, int l, int tbsiz, char **tb2, int *tb2_max)
{
    struct stat s;
    int tmp;

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c && j * l + i < tbsiz; j++)
        { // if options -s, -p, -m, -u or -g is set
            // then call "func" to printout the size, the permissions, the modification time, etc...
            if (tb2)
            {
                long_selector(tb2[j * l + i], tb2_max[j] - 1);
            }
            strcpy(&path[pathsiz], tb[j * l + i]);
            lf_stat(path, &s);
            lf_print(tb[j * l + i], &s.st_mode);
            tmp = tb_max[j] - strlen(tb[j * l + i]);
            for (int k = 0; k < tmp; k++)
            { // the +1 is for the last space between columns.
                printf(" ");
            }

            if (j != tbsiz - 1)
            { // don't add space at the last column
                printf("  ");
            }
        }
        printf("\n");
    }
}

void column_main(char **tb, char **tb2, int tbsiz)
{
    struct winsize w;
    int c = tbsiz, l = 1, winsiz, ok = 0, cnt;
    int *tb_sizes, *tb_max;
    int *tb2_sizes = NULL, *tb2_max = NULL;
    int tmp, tmp2 = 0;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    winsiz = w.ws_col;
    tb_sizes = (int *)lf_malloc(sizeof(int) * tbsiz);
    tb_max = (int *)lf_malloc(sizeof(int) * tbsiz);
    if (tb2)
    {
        tb2_sizes = (int *)lf_malloc(sizeof(int) * tbsiz);
        tb2_max = (int *)lf_malloc(sizeof(int) * tbsiz);
    }
    // length of each name.
    for (int i = 0; i < tbsiz; i++)
    {
        if (has_space(tb[i]))
        { // if name has space add +2, for the ""
            tb_sizes[i] = strlen(tb[i]) + 2;
        }
        else
        {
            tb_sizes[i] = strlen(tb[i]);
        }
        if (tb2)
        {
            tb2_sizes[i] = strlen(tb2[i]) + 1; // +1 for space between tb and tb2
        }
    }
    while (!ok)
    {
        cnt = 0;
        for (int i = 0; i < c; i++)
        { // for each column "i", calculates the maximum of that column
            tmp = tb_sizes[i * l];
            if (tb2)
            {
                tmp2 = tb2_sizes[i * l];
            }
            for (int j = 1; j < l && i * l + j < tbsiz; j++)
            {
                if (tb_sizes[i * l + j] > tmp)
                {
                    tmp = tb_sizes[i * l + j];
                }
                if (tb2 && (tb2_sizes[i * l + j] > tmp2))
                {
                    tmp2 = tb2_sizes[i * l + j];
                }
            }
            cnt += tmp + tmp2;
            tb_max[i] = tmp;
            if (tb2)
            {
                tb2_max[i] = tmp2;
            }
        }
        cnt += 2 * (c - 1); // space between columns
        if (cnt < winsiz)
        {
            ok = 1;
        }
        else
        {
            c--;
            if (tbsiz % c)
            {
                l = tbsiz / c + 1;
            }
            else
            {
                l = tbsiz / c;
            }
        }
    }
    free(tb_sizes);
    free(tb2_sizes);
    column_display(tb, tb_max, c, l, tbsiz, tb2, tb2_max);
    free(tb_max);
    free(tb2_max);
}
