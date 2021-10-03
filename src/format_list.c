#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list.h"
#include "format_list.h"
#include "common.h"
#include "display.h"
#include "types.h"
#include "format_long.h"

void list_main(linklist l, _file_info *files_info, int index)
{
    struct winsize w;
    int window_size;
    _file file;
    int nb_spaces;
    int nbr, remain;
    int i = 0;
    _bool beginning_of_line;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    window_size = w.ws_col;
    remain = window_size;

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        beginning_of_line = _false;
        file = (_file)it->data;
        nbr = strlen(file->name) + 1; // +1 for the character in use.
        nb_spaces = has_space(file->name);
        if (nb_spaces)
        {
            if (_opt.nl->b)
            {
                nbr += nb_spaces;
            }
            if (_opt.nl->q || !_opt.nl->b)
            {
                nbr += 2;
            }
        }
        else if (_opt.nl->q)
        {
            nbr += 2;
        }
        if (_opt.nl->i && (S_ISDIR(file->st.st_mode) ||
                           S_ISLNK(file->st.st_mode) ||
                           S_ISSOCK(file->st.st_mode) ||
                           S_ISFIFO(file->st.st_mode) ||
                           (S_IEXEC & file->st.st_mode)))
        {
            nbr += 1;
        }
        if (files_info)
        {
            nbr += strlen(files_info[i].bfr[index]) + 1; // +1 for space between information column and "l"
        }
        if (nbr < remain)
        {
            remain -= nbr;
        }
        else
        { // go to newline
            beginning_of_line = _true;
            remain = window_size - nbr;
            if (!_opt._1)
            {
                printf("\n");
            }
        }
        if (i && !beginning_of_line && !_opt._1 && !_opt._2)
        {
            printf(" ");
            --remain;
        }
        if (files_info)
        {
            printf("%s ", files_info[i].bfr[index]);
        }
        display(file->name, &file->st.st_mode, _false);
        if (_opt._1)
        {
            printf("\n");
        }
        else if (_opt._3)
        {
            printf(",");
        }
        else if (_opt._4)
        {
            printf(";");
        }
        else
        { // if _2

            printf(" ");
        }
    }
    if (!_opt._1)
    {
        printf("\n");
    }
}