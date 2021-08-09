#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"

lfoptions opt;
char *path, *buf;
int pathsiz, clr = 0, fl = 0;
linklist lcolor;

bool is_digit(const char *nm, int n)
{
    if (!nm || n <= 0)
    {
        return false;
    }
    while (n)
    {
        if (*nm < '0' || *nm > '9')
        {
            break;
        }
        --n;
    }
    return (n) ? false : true;
}

void small_options(int argc, char **argv, int *i)
{
    int tmp;
    char c;

    tmp = strlen(argv[*i]);
    for (int j = 1; j < tmp; j++)
    {
        c = argv[*i][j];
        if (c == 't')
        {
            opt.t = 1;
            if (j == tmp - 1)
            {
                if (*i < argc - 1)
                {
                    if (is_digit(argv[*i + 1], strlen(argv[*i + 1])))
                    {
                        opt.tdeep = strtol(argv[*i + 1], NULL, 10);
                        ++(*i);
                    }
                    else
                    {
                        opt.tdeep = 0;
                    }
                }
                else
                {
                    opt.tdeep = 0;
                }
            }
            else
            {
                opt.tdeep = 0;
            }
        }
        else if (c == 'a')
        {
            opt.a = 1;
        }
        else if (c == 'l')
        {
            opt.l = 1;
        }
        else if (c == 'c')
        {
            opt.c = 1;
        }
        else if (c == 'd')
        {
            opt.d = 1;
        }
        else if (c == 'f')
        {
            opt.f = 1;
        }
        else if (c == 'p')
        {
            opt.p = 1;
        }
        else if (c == 'm')
        {
            opt.m = 1;
        }
        else if (c == 's')
        {
            opt.s = 1;
        }
        else if (c == 'g')
        {
            opt.g = 1;
        }
        else if (c == 'u')
        {
            opt.u = 1;
        }
        else if (c == 'h')
        {
            opt.h = 1;
        }
        else
        {
        printf("%s: Invalid option \"%c\"\n", PROGRAM, c);
        }
    }
}

void long_options(const char *arg)
{
    if (!strcmp(arg, "--help"))
    {
        opt.help = 1;
    }
    else if (!strcmp(arg, "--version"))
    {
        opt.version = 1;
    }
    else
    {
        printf("%s: Invalid option \"%s\"\n", PROGRAM, arg);
    }
}

void set_arguments(int argc, char *argv[], linklist l)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == '-')
            {
                long_options(argv[i]);
            }
            else
            {
                small_options(argc, argv, &i);
            }
        }
        else
        {
            ladd(l, LLAST, argv[i]);
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    linklist l = lopen();

    lf_init();
    set_arguments(argc, argv, l);
    if (lempty(l))
    {
        ladd(l, LFIRST, "./");
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
        printf("%s: the option 't' cannot be used with 'f', 'd', 'l', 'p', 's', 'u', 'g' or 'm'.\n", PROGRAM);
        lf_quit();
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
            setbuf(stdout, NULL);
            if (!(lcolor = scan_for_color()))
            {
                printf("%s: cannot use \"-c\" enavailable because the environment variable \"LS_COLORS\" is not defined.\n", PROGRAM);
                lf_quit();
            }
        }
        run(l);
    }
    lf_quit(l);
    lclose(l);
    return 0;
}