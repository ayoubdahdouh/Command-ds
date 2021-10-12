#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <locale.h>
#include <errno.h>
#include "linkedList.h"
#include "ds.h"
#include "common.h"
#include "color.h"
#include "args.h"
#include "display.h"
#include "useLong.h"

u_int8_t Nparams;
u_int16_t Sparams;
u_int16_t Lparams;
u_int32_t Mparams;
u_int16_t Opts;

int Tparam;
char *Pth = NULL;
char *Bfr = NULL;
int PthLen = 0;
LinkedList ColorsList = NULL;
char *TimeStyle = NULL;

void run(LinkedList l)
{
    char *nm;
    int len;
    struct stat s;
    TreeInfo tree;
    File file;
    LinkedList filesList = lOpen();
    long int mularg = (l->count == 1) ? 0 : l->count;

    initProgram();

    if (Opts & OT)
    {
        tree.level = 0;
        tree.has_next = (char *)memAlloc(PATH_MAX);
    }
    for (Iterator it = lAt(l, LFIRST); it; lInc(&it))
    {
        nm = (char *)it->data;
        len = strlen(nm);
        if (len != 1 && nm[len - 1] == '/')
        {
            nm[--len] = 0;
        }
        // if multiple arguments then print name of each argument before linkedList.
        if (mularg)
        {
            printf("%s:\n", nm);
            --mularg;
        }
        if (fileStat(nm, &s))
        {
            if ((S_ISDIR(s.st_mode) || S_ISLNK(s.st_mode)) && !(Opts & OD))
            {
                if (S_ISDIR(s.st_mode))
                {
                    strcpy(Pth, nm);
                    PthLen = len;
                }
                else
                {
                    if (!readLink(nm))
                    {
                        printf("%s: Cannot read the reference of '%s': %s\n", PROGRAM, nm, strerror(errno));
                        continue;
                    }
                    strcpy(Pth, Bfr);
                    len = strlen(Bfr);
                    if (len != 1 && nm[len - 1] == '/')
                    {
                        nm[--len] = 0;
                    }
                    PthLen = len;
                    if (!fileStat(Bfr, &s))
                    {
                        printf("%s: Cannot access the reference of '%s': %s\n", PROGRAM, nm, strerror(errno));
                        continue;
                    }
                }
                if (Opts & OT)
                {
                    display(Pth, &s.st_mode, True);
                }
                if (Pth[PthLen - 1] != '/')
                {
                    Pth[PthLen++] = '/';
                    Pth[PthLen] = 0;
                }
                core(&tree);
            }
            else
            {
                if (Opts & OL)
                {
                    lReset(filesList);
                    file = (File)memAlloc(FILE_SIZE);
                    file->name = nm;
                    file->st = s;
                    lInsert(filesList, LFIRST, file);
                    longMain(filesList);
                }
                else
                {
                    display(nm, &s.st_mode, True);
                }
            }
            if (mularg)
            {
                printf("\n");
            }
        }
        else
        {
            printf("%s: '%s': %s\n", PROGRAM, nm, strerror(errno));
        }
    }
    lClose(filesList);
}

Bool isSingleCol()
{
    return (countActiveBits(Lparams, 10) == 1) ? True : False;
}

int main(int argc, char *argv[], char *envp[])
{
    LinkedList l = lOpen();

    setlocale(LC_ALL, "");

    setOptions(argc, argv, l);

    if ((Opts & OL) && countActiveBits(Opts, 5))
    {
        exitProgram(PROGRAM
                    ": Option 'l' can't be used with options"
                    " 't', '1', '2', '3' and '4'.");
    }

    if (Opts & OT)
    {
        if ((Tparam <= 0) || (Tparam > TREEMAXDEPTH))
        {
            printf("%s: The depth of the tree should be between 1 and %d.\n",
                   PROGRAM, TREEMAXDEPTH);
            exitProgram(NULL);
        }
    }

    if (Opts & OH)
    {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    if (Opts & OV)
    {
        printVersion();
        exit(EXIT_SUCCESS);
    }

    if (Opts & ON)
    {
        if (Nparams)
        { // -n with parameters
            // disable backslash if quoting is enabled.
            if ((Nparams & NB) && (Nparams & NQ))
            {
                Nparams &= ~NB;
            }
        }
        else
        { // -n without parameters
            // color names + quote spaced names + print also links targets if long format or tree format is set
            Nparams = NC | NQ;
            if (((Opts & OL) && !isSingleCol()) || (Opts & OT))
            {
                Nparams |= NF;
            }
        }
    }

    if (Nparams & NC)
    {
        ColorsList = scanForColors();
        if (lEmpty(ColorsList))
        {
            printf(PROGRAM
                   ": warning: '-c' needs the environment"
                   " variable 'LS_COLORS'.\n");
            Nparams &= ~NC;
        }
        else if (!getColor(ColorsList, "rs", False))
        { // LS_COLORS must have at least "rs" value
            printf(PROGRAM
                   ": warning: '-c' unavailable due to the environment"
                   " variable 'LS_COLORS' has no value 'rs'.\n");
            Nparams &= ~NC;
        }
    }

    if (Opts & OL)
    {
        if (!Lparams)
        {
            Lparams = LN | LP | LS | LR | LU | LG | LM;
        }
    }

    if (Opts & OM)
    { // -m is set
        if (Mparams)
        { // -m with parameters
            if (!(Mparams & (MD | MR | ML | MB | MC | MP | MS)))
            { // files types are not specified
                Mparams = MD | MR | ML | MB | MC | MP | MS;
            }
        }
        else
        { // -m without parameters
            Mparams = MH | MD | MR | ML | MB | MC | MP | MS;
        }
    }
    else
    {
        Mparams = MD | MR | ML | MB | MC | MP | MS;
    }

    if (!TimeStyle)
    {
        TimeStyle = "%F %R";
    }

    // default folder to linkedList is current working directory.
    if (lEmpty(l))
    {
        lInsert(l, LFIRST, ".");
    }
    run(l);
    exitProgram(NULL);
    lClose(l);
    return 0;
}