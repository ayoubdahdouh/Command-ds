#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"

void lf_show_with_color(const char *nm, mode_t *m, bool nl)
{
    struct stat s;
    int lk = 0; // is file link?
    char *color, *color2, *reset;
    reset = getcolor(lcolor, "rs", 0);

    switch (*m & S_IFMT)
    {
    case S_IFDIR:
        // if directory, blue colour
        color = getcolor(lcolor, "di", 0);
        color2 = reset;
        break;
    case S_IFLNK:
        if (fl)
        {
            lk = 1;
            strcpy(&path[pathsiz], nm);
            if (lf_link(path))
            {
                if (!is_absolute_path(buf))
                {
                    strcpy(&path[pathsiz], buf);
                    strcpy(buf, path);
                }
                if (lf_stat(buf, &s))
                {
                    if (S_ISDIR(s.st_mode))
                    { // if link and readpath are directories, blue colour
                        color = getcolor(lcolor, "ln", 0);
                        color2 = getcolor(lcolor, "di", 0);
                    }
                    else if (S_IXUSR & s.st_mode)
                    {
                        color = getcolor(lcolor, "ln", 0);
                        color2 = getcolor(lcolor, "ex", 0);
                    }
                    else if (S_IXUSR & *m)
                    { // if link executable, green colour
                        color = getcolor(lcolor, "ln", 0);
                        color2 = reset;
                    }
                    strcpy(buf, &path[pathsiz]);
                }
                else
                {
                    color = getcolor(lcolor, "or", 0);
                    color2 = reset;
                }
            }
        }
        else
        {
            color = getcolor(lcolor, "ln", 0);
            color2 = reset;
        }
        break;
    case S_IFREG:
        if (*m & S_IXUSR)
        { // if executable, green colour
            color = getcolor(lcolor, "ex", 0);
            color2 = reset;
        }
        else
        {
            char *ext = lfext(nm);
            if (ext)
            {
                color = getcolor(lcolor, ext, true);
            }
            else
            {
                color = reset;
            }

            color2 = reset;
        }
        break;
    default:
        color = color2 = reset;
        break;
    }
    if (has_space(nm))
    {
        printf("\033[%sm\"%s\"\033[%sm", color, nm, reset);
    }
    else
    {
        printf("\033[%sm%s\033[%sm", color, nm, reset);
    }
    if (lk)
    {
        if (has_space(buf))
        {
            printf(" -> \033[%sm\"%s\"\033[%sm", color2, buf, reset);
        }
        else
        {
            printf(" -> \033[%sm%s\033[%sm", color2, buf, reset);
        }
    }
    if (nl)
    {
        printf("\n");
    }
}

void lf_show(char *nm, mode_t *m, bool nl)
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
    if (clr)
    {
        lf_show_with_color(nm, m, nl);
        return;
    }
    lk = 0;
    if (fl && ((*m & S_IFMT) == S_IFLNK))
    {
        lk = 1;
        strcpy(&path[pathsiz], nm);
        if (lf_link(path))
        {
            if (!is_absolute_path(buf))
            {
                strcpy(&path[pathsiz], buf);
                strcpy(buf, path);
            }
            if (lf_stat(buf, &s))
            {
                strcpy(buf, &path[pathsiz]);
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
        if (has_space(buf))
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> \"%s\"/", buf);
            }
            else
            {
                printf(" -> \"%s\"", buf);
            }
        }
        else
        {
            if (S_ISDIR(s.st_mode))
            {
                printf(" -> %s/", buf);
            }
            else
            {
                printf(" -> %s", buf);
            }
        }
    }
    if (nl)
    {
        printf("\n");
    }
}
