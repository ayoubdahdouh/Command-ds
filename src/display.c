#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"
void display_name(char *nm, mode_t *m, char *c, char *r)
{
    int i = 0;
    if (LFopt.c && (!c || !r))
    {
        // if "c" and "r" aren't set then disable coloring output
        LFopt.c = false;
    }
    if (LFopt.c)
    {
        sprintf(LFbuf, "\033[%sm", c);
        i = strlen(LFbuf);
    }
    if (LFopt.nl->q || (!LFopt.nl->b && has_space(nm)))
    {
        LFbuf[i++] = '"';
    }
    if (LFopt.nl->b)
    {
        for (char *c = nm; *c; ++c)
        {
            if (*c == ' ')
            {
                LFbuf[i++] = '\\';
            }
            LFbuf[i++] = *c;
        }
    }
    else
    {
        strcpy(&LFbuf[i], nm);
        i = strlen(LFbuf);
    }

    if (LFopt.nl->q || (!LFopt.nl->b && has_space(nm)))
    {
        LFbuf[i++] = '"';
    }
    if (S_ISDIR(*m) && LFopt.nl->s)
    {
        LFbuf[i++] = '/';
    }
    LFbuf[i] = 0;
    if (LFopt.c)
    {
        sprintf(&LFbuf[i], "\033[%sm", r);
    }
    printf("%s", LFbuf);
}

void choose_color(char *nm, mode_t *m, char **c)
{
    if (!nm)
    {
        *c = getcolor(LFcolorlist, nm, true);
    }
    if (S_ISBLK(*m))
    {
        *c = getcolor(LFcolorlist, "bd", false);
    }
    else if (S_ISCHR(*m))
    {
        *c = getcolor(LFcolorlist, "cd", false);
    }
    else if (S_ISFIFO(*m))
    {
        *c = getcolor(LFcolorlist, "pi", false);
    }
    else if (S_ISSOCK(*m))
    {
        *c = getcolor(LFcolorlist, "so", false);
    }
    else if (S_ISDIR(*m))
    {
        *c = getcolor(LFcolorlist, "di", false);
    }
    else if (S_ISLNK(*m))
    {
        *c = getcolor(LFcolorlist, "ln", false);
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
        //             *c1 = *c2 = getcolor(LFcolorlist, "or", false);
        //         }
        //         strcpy(LFpath, LFbuf);
        //         if (!*c1)
        //         {
        //             *c1 = getcolor(LFcolorlist, "ln", false);
        //             choose_color(LFbuf, &s.st_mode, true, c2, NULL, NULL);
        //         }
        //     }
        // }
    }
    else if (S_ISREG(*m))
    {
        if (*m & S_ISUID)
        { // if executable, green colour
            *c = getcolor(LFcolorlist, "su", false);
        }
        else if (*m & S_ISGID)
        { // if executable, green colour
            *c = getcolor(LFcolorlist, "sg", false);
        }
        else if (*m & S_ISVTX)
        { // if executable, green colour
            *c = getcolor(LFcolorlist, "tw", false);
        }
        else if (*m & S_IXUSR)
        { // if executable, green colour
            *c = getcolor(LFcolorlist, "ex", false);
        }
        else
        {
            char *ext = fileextension(nm);
            if (ext)
            {
                *c = getcolor(LFcolorlist, ext, true);
            }
            else
            {
                *c = getcolor(LFcolorlist, "rs", false);
            }
        }
    }
    else
    {
        *c = getcolor(LFcolorlist, "rs", false);
    }
}

void display(char *nm, mode_t *m, bool nl)
{
    struct stat s;
    char *c1 = NULL, *c2 = NULL, *rs = NULL;

    if (LFopt.c)
    {
        rs = getcolor(LFcolorlist, "rs", false);
        choose_color(nm, m, &c1);
    }
    if (S_ISLNK(*m))
    {
        strcpy(&LFpath[LFpathsiz], nm);
        if (lf_link(LFpath))
        {
            if (!is_absolute_path(LFbuf))
            {
                strcpy(&LFpath[LFpathsiz], LFbuf);
                strcpy(LFbuf, LFpath);
            }
            if (!lf_stat(LFbuf, &s) && LFopt.c)
            {
                c1 = c2 = getcolor(LFcolorlist, "or", false);
            }
            else if (LFopt.c)
            {
                choose_color(LFbuf, &s.st_mode, &c2);
            }
            strcpy(LFbuf, &LFpath[LFpathsiz]);
        }
        else if (LFopt.c)
        {
            c1 = c2 = getcolor(LFcolorlist, "or", false);
        }
    }
    display_name(nm, m, c1, rs);
    if (S_ISLNK(*m) && LFopt.nl->f)
    {
        printf(" -> ");
        display_name(LFbuf, &s.st_mode, c2, rs);
    }
    if (nl)
    {
        printf("\n");
    }
}