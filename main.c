#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"

lfoptions opt;
char *path, *buf;
int pathsiz, clr = 0, fl = 0;
LIST lcolor;

void lf_set_option(char c)
{
    switch (c)
    {
    case 'a':
        opt.a = 1;
        break;
    case 'c':
        opt.c = 1;
        break;
    case 'd':
        opt.d = 1;
        break;
    case 'f':
        opt.f = 1;
        break;
    case 'g':
        opt.g = 1;
        break;
    case 'h':
        opt.h = 1;
        break;
    case 'l':
        opt.l = 1;
        break;
    case 'm':
        opt.m = 1;
        break;
    case 'p':
        opt.p = 1;
        break;
    case 's':
        opt.s = 1;
        break;
    case 't':
        opt.t = 1;
        break;
    case 'u':
        opt.u = 1;
        break;
    default:
        lf_error(ERR_INVALID_OPTION, &c, true);
        break;
    }
}

void lf_set_arguments(int argc, char *argv[], LIST l)
{
    int tmp;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == '-')
            {
                if (!strcmp(argv[i], "--help"))
                {
                    opt.help = 1;
                }
                else if (!strcmp(argv[i], "--version"))
                {
                    opt.version = 1;
                }
                else
                {
                    lf_error(ERR_INVALID_OPTION, argv[i], true);
                }
            }
            else
            {
                tmp = strlen(argv[i]);
                for (int j = 1; j < tmp; j++)
                {
                    lf_set_option(argv[i][j]);
                }
            }
        }
        else
        {
            LADD(l, LLAST, argv[i]);
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    LIST l = LOPEN();

    lf_init();
    lf_set_arguments(argc, argv, l);
    if (LEMPTY(l))
    {
        LADD(l, LFIRST, "./");
    }
    if (opt.help)
    {
        help();
    }
    else if (opt.version)
    {
        version();
    }
    else if (opt.t && (opt.f || opt.d || opt.l || opt.p || opt.s || opt.u || opt.g || opt.m))
    {
        printf("Warning: The option 't' cannot be used with 'f', 'd', 'l', 'p', 's', 'u', 'g' or 'm'.\n");
    }
    else
    {
        if (!opt.f && !opt.d)
        {
            opt.f = opt.d = 1;
        }
        if (opt.l || opt.t)
        {
            fl = 1;
        }
        if (opt.c)
        {
            clr = 1;
            if (getenv("LS_COLORS"))
            {
                lcolor = scan_for_color();
                // for (ITERATOR i = LAT(lcolor,LFIRST); i ; LINC(&i))
                // {
                //     printf("%s\t%s\n", ((lfcolor*)i->data)->a, ((lfcolor*)i->data)->c);
                // }
                // exit(EXIT_FAILURE);
            }
            else
            {
                lf_error(ERR_COLORS_NOT_AVILABLE, NULL, true);
            }
        }
        run(l);
    }
    lf_quit(l);
    LCLOSE(l);
    return 0;
}