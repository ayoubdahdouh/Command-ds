#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "lf.h"
#include "common.h"

lfoptions opt;
char *path;
int pathsiz;
int with_colors;
char *buf;

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
        lf_error(ERR_INVALID_OPTION, &c);
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
                    lf_error(ERR_INVALID_OPTION, argv[i]);
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
    LIST l;

    setbuf(stdout, NULL);
    path = (char *)lfalloc(sizeof(char) * PATH_MAX);
    buf = (char *)lfalloc(sizeof(char) * PATH_MAX);
    memset(&opt, 0, OPTIONSIZ);

    l = LOPEN();
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
        run(l);
    }
    LCLOSE(l);
    free(buf);
    free(path);
    return 0;
}