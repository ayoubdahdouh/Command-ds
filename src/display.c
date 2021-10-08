#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"
void display_name(char *nm, mode_t *m, char *c, char *r)
{
    int n = countSpaces(nm);

    if (Nparams & NC)
    {
        printf("\033[%sm", c);
    }

    if ((Nparams & NQ) && n)
    {
        printf("\"");
    }
    // if name "nm" contains spaces and print bachslash is enabled.
    if ((Nparams & NB) && n)
    {
        for (char *c = nm; *c; ++c)
        {
            if (*c == ' ')
            {
                printf("\\ ");
            }
            else
            {
                printf("%c", *c);
            }
        }
    }
    else
    {
        printf("%s", nm);
    }

    if ((Nparams & NQ) && n)
    {
        printf("\"");
    }
    if (Nparams & NC)
    {
        printf("\033[%sm", r);
    }
    if (Nparams & NI)
    {
        if (S_ISDIR(*m))
        {
            printf("/");
        }
        else if (S_ISLNK(*m))
        {
            printf("@");
        }
        else if (S_IEXEC & *m)
        {
            printf("*");
        }
        else if (S_ISFIFO(*m))
        {
            printf("|");
        }
        else if (S_ISSOCK(*m))
        {
            printf("=");
        }
    }
}

void display(char *nm, mode_t *m, Bool nl)
{
    struct stat s;
    char *c1 = NULL, *c2 = NULL, *rs = NULL;

    if (Nparams & NC)
    {
        rs = getcolor(ColorsList, "rs", False);
        choose_color(nm, m, &c1);
    }
    if (S_ISLNK(*m))
    {
        strcpy(&Pth[PthLen], nm);
        if (readLink(Pth))
        {
            if (!isAbsolutePath(Bfr))
            {
                strcpy(&Pth[PthLen], Bfr);
                strcpy(Bfr, Pth);
            }
            if (Stat(Bfr, &s))
            {
                if (Nparams & NC)
                {
                    choose_color(Bfr, &s.st_mode, &c2);
                }
            }
            else if (Nparams & NC)
            {
                c1 = c2 = getcolor(ColorsList, "or", False);
            }
            strcpy(Bfr, &Pth[PthLen]);
        }
        else if (Nparams & NC)
        {
            c1 = c2 = getcolor(ColorsList, "or", False);
        }
    }

    display_name(nm, m, c1, rs);
    if (S_ISLNK(*m) && Nparams & NF)
    {
        printf(" -> ");
        display_name(Bfr, &s.st_mode, c2, rs);
    }

    if (nl)
    {
        printf("\n");
    }
}

void choose_color(char *nm, mode_t *m, char **c)
{
    if (!nm)
    {
        *c = getcolor(ColorsList, nm, True);
    }
    if (S_ISBLK(*m))
    {
        *c = getcolor(ColorsList, "bd", False);
    }
    else if (S_ISCHR(*m))
    {
        *c = getcolor(ColorsList, "cd", False);
    }
    else if (S_ISFIFO(*m))
    {
        *c = getcolor(ColorsList, "pi", False);
    }
    else if (S_ISSOCK(*m))
    {
        *c = getcolor(ColorsList, "so", False);
    }
    else if (S_ISDIR(*m))
    {
        *c = getcolor(ColorsList, "di", False);
    }
    else if (S_ISLNK(*m))
    {
        *c = getcolor(ColorsList, "ln", False);
        // if (follow_lnk)
        // {

        // }
        // else
        // {
        //     strcpy(&LFpath[LFpathsiz], nm);
        //     if (lf_link(LFpath))
        //     {
        //         if (!is_absolute_path(LFbuf))
        //         {
        //             strcpy(&LFpath[LFpathsiz], LFbuf);
        //             strcpy(LFbuf, LFpath);
        //         }
        //         if (!lf_stat(LFbuf, &s))
        //         {
        //             *c1 = *c2 = getcolor(LFcolorlist, "or", False);
        //         }
        //         strcpy(LFpath, LFbuf);
        //         if (!*c1)
        //         {
        //             *c1 = getcolor(LFcolorlist, "ln", False);
        //             choose_color(LFbuf, &s.st_mode, True, c2, NULL, NULL);
        //         }
        //     }
        // }
    }
    else if (S_ISREG(*m))
    {
        if (*m & S_ISUID)
        { // if executable, green colour
            *c = getcolor(ColorsList, "su", False);
        }
        else if (*m & S_ISGID)
        { // if executable, green colour
            *c = getcolor(ColorsList, "sg", False);
        }
        else if (*m & S_ISVTX)
        { // if executable, green colour
            *c = getcolor(ColorsList, "tw", False);
        }
        else if (*m & S_IXUSR)
        { // if executable, green colour
            *c = getcolor(ColorsList, "ex", False);
        }
        else
        {
            char *ext = fileExtension(nm);
            if (ext)
            {
                *c = getcolor(ColorsList, ext, True);
            }
            else
            {
                *c = getcolor(ColorsList, "rs", False);
            }
        }
    }
    else
    {
        *c = getcolor(ColorsList, "rs", False);
    }
}
