#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"

lfoptions LF_opt;
char *LF_path, *LF_buf;
int LF_pathsiz;
bool LF_clr = false, LF_fl = false;
linklist LF_lcolor;

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
            LF_opt.t = true;
            if (j == tmp - 1)
            {
                if (*i < argc - 1)
                {
                    if (is_digit(argv[*i + 1], strlen(argv[*i + 1])))
                    {
                        LF_opt.tdeep = strtol(argv[*i + 1], NULL, 10);
                        ++(*i);
                    }
                    else
                    {
                        LF_opt.tdeep = 0;
                    }
                }
                else
                {
                    LF_opt.tdeep = 0;
                }
            }
            else
            {
                LF_opt.tdeep = 0;
            }
        }
        else if (c == 'a')
        {
            LF_opt.a = true;
        }
        else if (c == 'c')
        {
            LF_opt.c = true;
        }
        else if (c == 'd')
        {
            LF_opt.d = true;
        }
        else if (c == 'f')
        {
            LF_opt.f = true;
        }
        else if (c == 'g')
        {
            LF_opt.g = true;
        }
        else if (c == 'i')
        {
            LF_opt.i = true;
        }
        else if (c == 'n')
        {
            LF_opt.n = true;
        }
        else if (c == 'l')
        {
            LF_opt.l = true;
        }
        else if (c == 'm')
        {
            LF_opt.m = true;
        }
        else if (c == 'p')
        {
            LF_opt.p = true;
        }
        else if (c == 's')
        {
            LF_opt.s = true;
        }
        else if (c == 'u')
        {
            LF_opt.u = true;
        }
        else if (c == 'h')
        {
            LF_opt.h = true;
        }
        else
        {
            printf("%s: Invalid option \"%c\"\n", PROGRAM, c);
        }
    }
}

void long_options(const char *arg)
{
    if (strcmp(arg, "--help") == 0)
    {
        LF_opt.help = true;
    }
    else if (strcmp(arg, "--version") == 0)
    {
        LF_opt.version = true;
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
    if (LF_opt.help)
    {
        help();
    }
    else if (LF_opt.version)
    {
        version();
    }
    else if (LF_opt.t && (LF_opt.f || LF_opt.d || LF_opt.i || LF_opt.n || LF_opt.u || LF_opt.g || LF_opt.l || LF_opt.m || LF_opt.p || LF_opt.s))
    {
        printf("%s: the option 't' cannot be used with 'f', 'd', 'i', 'n', 'l', 'p', 's', 'u', 'g' or 'm'.\n", PROGRAM);
        lf_quit();
    }
    else
    {
        if (!LF_opt.f && !LF_opt.d)
        {
            LF_opt.f = LF_opt.d = true;
        }
        if (LF_opt.l || LF_opt.t)
        {
            LF_fl = true;
        }
        if (LF_opt.c)
        {
            if (!(LF_lcolor = scan_for_color()))
            {
                printf("%s: warning: \"-c\" not available because the \"LS_COLORS\" environment variable is not set.\n", PROGRAM);
                LF_clr = false;
            }
            else if (!getcolor(LF_lcolor, "rs", false))
            { // at least LS_COLORS must have value for "rs"
                printf("%s: warning: \"-c\" not available because the environment variable \"LS_COLORS\" has no value \"rs\".\n", PROGRAM);
                LF_clr = false;
            }
            else
            {
                LF_clr = true;
            }
        }
        run(l);
    }
    lf_quit(l);
    lclose(l);
    return 0;
}