#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"

void lfprint_colored(const char *nm, mode_t *m, bool nl, bool fl)
{
    struct stat s;
    char *rs = getcolor(LF_lcolor, "rs", false);
    char *o = getcolor(LF_lcolor, "or", false);
    char *c = NULL;
    char r = filetype(m);
    bool lnk = false; // is file link?

    switch (r)
    {
    case 'b':
        c = getcolor(LF_lcolor, "bd", false);
        break;
    case 'c':
        c = getcolor(LF_lcolor, "cd", false);
        break;
    case 'p':
        c = getcolor(LF_lcolor, "pi", false);
        break;
    case 's':
        c = getcolor(LF_lcolor, "so", false);
        break;
    case 'd':
        c = getcolor(LF_lcolor, "di", false);
        break;
    case 'l':
        lnk = true;
        strcpy(&LF_path[LF_pathsiz], nm);
        if (lf_link(LF_path))
        {
            if (!is_absolute_path(LF_buf))
            {
                strcpy(&LF_path[LF_pathsiz], LF_buf);
                if (!lf_stat(LF_path, &s))
                {
                    c = o;
                }
            }
            else if (!lf_stat(LF_buf, &s))
            {
                c = o;
            }
        }
        if (!c)
        {
            c = getcolor(LF_lcolor, "ln", false);
        }
        break;
    case '~':
        if (*m & S_ISUID)
        { // if executable, green colour
            c = getcolor(LF_lcolor, "su", false);
        }
        else if (*m & S_ISGID)
        { // if executable, green colour
            c = getcolor(LF_lcolor, "sg", false);
        }
        else if (*m & S_ISVTX)
        { // if executable, green colour
            c = getcolor(LF_lcolor, "tw", false);
        }
        else if (*m & S_IXUSR)
        { // if executable, green colour
            c = getcolor(LF_lcolor, "ex", false);
        }
        else
        {
            char *ext = lfext(nm);
            if (ext)
            {
                c = getcolor(LF_lcolor, ext, true);
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
            if (has_space(LF_buf))
            {
                printf(" -> \033[%sm\"%s\"\033[%sm", c, LF_buf, rs);
            }
            else
            {
                printf(" -> \033[%sm%s\033[%sm", c, LF_buf, rs);
            }
        }
        else
        {
            printf(" -> ");
            lfprint_colored(LF_buf, &s.st_mode, false, false);
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
    if (LF_clr)
    {
        lfprint_colored(nm, m, nl, fl);
        return;
    }
    lk = 0;
    if (fl && ((*m & S_IFMT) == S_IFLNK))
    {
        lk = 1;
        strcpy(&LF_path[LF_pathsiz], nm);
        if (lf_link(LF_path))
        {
            if (!is_absolute_path(LF_buf))
            {
                strcpy(&LF_path[LF_pathsiz], LF_buf);
                strcpy(LF_buf, LF_path);
            }
            if (lf_stat(LF_buf, &s))
            {
                strcpy(LF_buf, &LF_path[LF_pathsiz]);
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
        if (has_space(LF_buf))
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> \"%s\"/", LF_buf);
            }
            else
            {
                printf(" -> \"%s\"", LF_buf);
            }
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> %s/", LF_buf);
            }
            else
            {
                printf(" -> %s", LF_buf);
            }
        }
    }
    if (nl)
    {
        printf("\n");
    }
}
