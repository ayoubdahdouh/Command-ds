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

void list_main(linklist l, FileInfo *files_info, int index)
{
    struct winsize w;
    int window_size;
    File file;
    int nb_spaces;
    int nbr, remain;
    int i = 0;
    Bool beginning_of_line;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    window_size = w.ws_col;
    remain = window_size;

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        beginning_of_line = False;
        file = (File)it->data;
        nbr = strlen(file->name) + 1; // +1 for the character in use.
        nb_spaces = countSpaces(file->name);
        if (nb_spaces)
        {
            if (Nparams & NB)
            {
                nbr += nb_spaces;
            }
            if (Nparams & NQ)
            {
                nbr += 2;
            }
        }
        else if (Nparams & NQ)
        {
            nbr += 2;
        }
        if (Nparams & NI && (S_ISDIR(file->st.st_mode) ||
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
            beginning_of_line = True;
            remain = window_size - nbr;
            if (!(Opts & O1))
            {
                printf("\n");
            }
        }
        if (i && !beginning_of_line && !(Opts & O1) && !(Opts & O2))
        {
            printf(" ");
            --remain;
        }
        if (files_info)
        {
            printf("%s ", files_info[i].bfr[index]);
        }
        display(file->name, &file->st.st_mode, False);
        if (Opts & O1)
        {
            printf("\n");
        }
        else if (Opts & O3)
        {
            printf(",");
        }
        else if (Opts & O4)
        {
            printf(";");
        }
        else
        { // if _2

            printf(" ");
        }
    }
    if (!(Opts & O1))
    {
        printf("\n");
    }
}