#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "linkedList.h"
#include "useList.h"
#include "common.h"
#include "display.h"
#include "types.h"
#include "useLong.h"

void listMain(LinkedList l, FileInfo *filesInfo, int index)
{
    struct winsize w;
    int wSize;
    File file;
    int nbrSpaces;
    int nbr, remain;
    int i = 0;
    Bool newLine;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    wSize = w.ws_col;
    remain = wSize;

    for (Iterator it = lAt(l, LFIRST); it; lInc(&it), ++i)
    {
        newLine = False;
        file = (File)it->data;
        nbr = strlen(file->name) + 1; // +1 for the character in use.
        nbrSpaces = countSpaces(file->name);
        if (nbrSpaces)
        {
            if (Nparams & NB)
            {
                nbr += nbrSpaces;
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
        if (filesInfo)
        {
            nbr += strlen(filesInfo[i].bfr[index]) + 1; // +1 for space between information column and "l"
        }
        if (nbr < remain)
        {
            remain -= nbr;
        }
        else
        { // go to newline
            newLine = True;
            remain = wSize - nbr;
            if (!(Opts & O1))
            {
                printf("\n");
            }
        }
        if (i && !newLine && !(Opts & O1) && !(Opts & O2))
        {
            printf(" ");
            --remain;
        }
        if (filesInfo)
        {
            printf("%s ", filesInfo[i].bfr[index]);
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