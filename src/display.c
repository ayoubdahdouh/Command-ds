#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "display.h"
#include "common.h"
#include "color.h"
void display_name(char *nm, mode_t *m, char *c, char *r)
{
    int n = has_space(nm);

    if (_opt.nl->c)
    {
        printf("\033[%sm", c);
    }

    if (_opt.nl->q && n)
    {
        printf("\"");
    }
    // if name "nm" contains spaces and print bachslash is enabled.
    if (_opt.nl->b && n)
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

    if (_opt.nl->q && n)
    {
        printf("\"");
    }
    if (_opt.nl->c)
    {
        printf("\033[%sm", r);
    }
    if (_opt.nl->i)
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

void display(char *nm, mode_t *m, _bool nl)
{
    struct stat s;
    char *c1 = NULL, *c2 = NULL, *rs = NULL;

    if (_opt.nl->c)
    {
        rs = getcolor(_colors_list, "rs", _false);
        choose_color(nm, m, &c1);
    }
    if (S_ISLNK(*m))
    {
        strcpy(&_path[_path_len], nm);
        if (_link(_path))
        {
            if (!is_absolute_path(_buffer))
            {
                strcpy(&_path[_path_len], _buffer);
                strcpy(_buffer, _path);
            }
            if (_stat(_buffer, &s))
            {
                if (_opt.nl->c)
                {
                    choose_color(_buffer, &s.st_mode, &c2);
                }
            }
            else if (_opt.nl->c)
            {
                c1 = c2 = getcolor(_colors_list, "or", _false);
            }
            strcpy(_buffer, &_path[_path_len]);
        }
        else if (_opt.nl->c)
        {
            c1 = c2 = getcolor(_colors_list, "or", _false);
        }
    }

    display_name(nm, m, c1, rs);
    if (S_ISLNK(*m) && _opt.nl->f)
    {
        printf(" -> ");
        display_name(_buffer, &s.st_mode, c2, rs);
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
        *c = getcolor(_colors_list, nm, _true);
    }
    if (S_ISBLK(*m))
    {
        *c = getcolor(_colors_list, "bd", _false);
    }
    else if (S_ISCHR(*m))
    {
        *c = getcolor(_colors_list, "cd", _false);
    }
    else if (S_ISFIFO(*m))
    {
        *c = getcolor(_colors_list, "pi", _false);
    }
    else if (S_ISSOCK(*m))
    {
        *c = getcolor(_colors_list, "so", _false);
    }
    else if (S_ISDIR(*m))
    {
        *c = getcolor(_colors_list, "di", _false);
    }
    else if (S_ISLNK(*m))
    {
        *c = getcolor(_colors_list, "ln", _false);
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
        //             *c1 = *c2 = getcolor(LFcolorlist, "or", _false);
        //         }
        //         strcpy(LFpath, LFbuf);
        //         if (!*c1)
        //         {
        //             *c1 = getcolor(LFcolorlist, "ln", _false);
        //             choose_color(LFbuf, &s.st_mode, _true, c2, NULL, NULL);
        //         }
        //     }
        // }
    }
    else if (S_ISREG(*m))
    {
        if (*m & S_ISUID)
        { // if executable, green colour
            *c = getcolor(_colors_list, "su", _false);
        }
        else if (*m & S_ISGID)
        { // if executable, green colour
            *c = getcolor(_colors_list, "sg", _false);
        }
        else if (*m & S_ISVTX)
        { // if executable, green colour
            *c = getcolor(_colors_list, "tw", _false);
        }
        else if (*m & S_IXUSR)
        { // if executable, green colour
            *c = getcolor(_colors_list, "ex", _false);
        }
        else
        {
            char *ext = file_ext(nm);
            if (ext)
            {
                *c = getcolor(_colors_list, ext, _true);
            }
            else
            {
                *c = getcolor(_colors_list, "rs", _false);
            }
        }
    }
    else
    {
        *c = getcolor(_colors_list, "rs", _false);
    }
}
