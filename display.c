#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"

void lfprint_colored(char *nm, mode_t *m, bool nl, bool fl)
{
    struct stat s;
    char *rs = getcolor(LFcolorlist, "rs", false);
    char *o = getcolor(LFcolorlist, "or", false);
    char *c = NULL;
    char r = filetype(m);
    bool lnk = false; // is file link?

    switch (r)
    {
    case 'b':
        c = getcolor(LFcolorlist, "bd", false);
        break;
    case 'c':
        c = getcolor(LFcolorlist, "cd", false);
        break;
    case 'p':
        c = getcolor(LFcolorlist, "pi", false);
        break;
    case 's':
        c = getcolor(LFcolorlist, "so", false);
        break;
    case 'd':
        c = getcolor(LFcolorlist, "di", false);
        break;
    case 'l':
        lnk = true;
        strcpy(&LFpath[LFpathsiz], nm);
        if (lf_link(LFpath))
        {
            if (!is_absolute_path(LFbuf))
            {
                strcpy(&LFpath[LFpathsiz], LFbuf);
                if (!lf_stat(LFpath, &s))
                {
                    c = o;
                }
            }
            else if (!lf_stat(LFbuf, &s))
            {
                c = o;
            }
        }
        if (!c)
        {
            c = getcolor(LFcolorlist, "ln", false);
        }
        break;
    case '~':
        if (*m & S_ISUID)
        { // if executable, green colour
            c = getcolor(LFcolorlist, "su", false);
        }
        else if (*m & S_ISGID)
        { // if executable, green colour
            c = getcolor(LFcolorlist, "sg", false);
        }
        else if (*m & S_ISVTX)
        { // if executable, green colour
            c = getcolor(LFcolorlist, "tw", false);
        }
        else if (*m & S_IXUSR)
        { // if executable, green colour
            c = getcolor(LFcolorlist, "ex", false);
        }
        else
        {
            char *ext = fileextension(nm);
            if (ext)
            {
                c = getcolor(LFcolorlist, ext, true);
            }
            else
            {
                c = rs;
            }
        }
        break;
    default:
        c = rs;
        break;
    }
    if (has_space(nm))
    {
        printf("\033[%sm\"%s\"\033[%sm", c, nm, rs);
    }
    else
    {
        printf("\033[%sm%s\033[%sm", c, nm, rs);
    }
    if (lnk && fl)
    {
        if (c == o)
        {
            if (has_space(LFbuf))
            {
                printf(" -> \033[%sm\"%s\"\033[%sm", c, LFbuf, rs);
            }
            else
            {
                printf(" -> \033[%sm%s\033[%sm", c, LFbuf, rs);
            }
        }
        else
        {
            printf(" -> ");
            lfprint_colored(LFbuf, &s.st_mode, false, false);
        }
    }
    if (nl)
    {
        printf("\n");
    }
}

void lfprint(char *nm, mode_t *m, bool nl, bool fl)
{
    struct stat s;
    int lk; // link
    if (!m)
    {
        printf("%s", nm);
        if (nl)
        {
            printf("\n");
        }
    }
    if (LF_use_color)
    {
        lfprint_colored(nm, m, nl, fl);
        return;
    }
    lk = 0;
    if (fl && ((*m & S_IFMT) == S_IFLNK))
    {
        lk = 1;
        strcpy(&LFpath[LFpathsiz], nm);
        if (lf_link(LFpath))
        {
            if (!is_absolute_path(LFbuf))
            {
                strcpy(&LFpath[LFpathsiz], LFbuf);
                strcpy(LFbuf, LFpath);
            }
            if (lf_stat(LFbuf, &s))
            {
                strcpy(LFbuf, &LFpath[LFpathsiz]);
            }
        }
    }
    if (has_space(nm))
    {
        if (S_ISDIR(*m))
        {
            printf("\"%s\"/", nm);
        }
        else
        {
            printf("\"%s\"", nm);
        }
    }
    else
    {
        if (S_ISDIR(*m))
        {
            printf("%s/", nm);
        }
        else
        {
            printf("%s", nm);
        }
    }
    if (lk)
    {
        if (has_space(LFbuf))
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> \"%s\"/", LFbuf);
            }
            else
            {
                printf(" -> \"%s\"", LFbuf);
            }
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> %s/", LFbuf);
            }
            else
            {
                printf(" -> %s", LFbuf);
            }
        }
    }
    if (nl)
    {
        printf("\n");
    }
}
