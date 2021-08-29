#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"

lf_option LFopt;
char *LFpath;
char *LFbuf;
int LFpathsiz;
linklist LFcolorlist;

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

int set_t_arg(char *s[], int n, int i, int *j)
{
    int depth = 0;
    bool ok = true;
    if ((*j < strlen(s[i]) - 1) &&
        (s[i][*j + 1] == '='))
    {
        *j += 2;
        int cnt = 0;
        for (char *k = &s[i][*j]; *k && *k != ','; ++k)
        {
            if (*k >= '0' && *k <= '9')
            {
                ++cnt;
            }
            else
            {
                ok = false;
                break;
            }
        }
        if (ok && cnt > 0)
        {
            char *ss = (char *)lf_alloc(cnt * sizeof(char));
            strncpy(ss, &s[i][*j], cnt);
            *j += cnt;
            depth = strtol(ss, NULL, 10);
            free(ss);
        }
        else
        {
            if (!ok)
            {
                printf("%s: For the \"t\" option, the argument must be a positive number.\n", PROGRAM);
            }
            else if (cnt == 0)
            {
                printf(PROGRAM ": The \"s\" option needs an argument after \"=\".\n");
            }
            else
            {
                printf(PROGRAM "Error in the option \"t\".");
            }
            printf("Please try \"" PROGRAM " -h\" for help.\n");
            lf_quit();
        }
    }
    return depth;
}

char set_s_arg(char *s[], int n, int i, int *j)
{
    char attr = 0;
    bool ok = true, diff = false;

    // if 's' is at  the end of string
    // and there's a argument
    if ((*j < strlen(s[i]) - 1) &&
        (s[i][*j + 1] == '='))
    {
        *j += 2;
        int cnt = 0;
        char *k, prev = 0;

        for (k = &s[i][*j]; *k && *k != ','; ++k)
        {
            if (!strchr("inugsamcte", *k))
            {
                ok = false;
                break;
            }
            else
            {
                if (prev == 0)
                {
                    prev = *k;
                }
                if (prev != *k)
                {
                    diff = true;
                    break;
                }
                ++cnt;
            }
        }
        if (ok && !diff && cnt > 0)
        {
            attr = s[i][*j];
            ++(*j);
        }
        else
        {
            if (!ok)
            {
                printf("%s: The \"s\" option doesn't recognize the argument \"%c\".\n", PROGRAM, *k);
            }
            else if (diff)
            {
                printf(PROGRAM ": The \"s\" option accepts only one argument.\n");
            }
            else if (cnt == 0)
            {
                printf(PROGRAM ": The \"s\" option needs an argument after \"=\".\n");
            }
            else
            {
                printf(PROGRAM "Error in the option \"s\".");
            }
            printf("Please try \"" PROGRAM " -h\" for help.\n");
            lf_quit();
        }
    }
    return attr;
}

m_arg *set_m_arg(char *s[], int n, int i, int *j)
{
    bool ok = true;
    m_arg *m = NULL;

    if ((*j < strlen(s[i]) - 1) &&
        (s[i][*j + 1] == '='))
    {
        *j += 2;
        int cnt = 0;
        char *k;
        // verify that data are correct
        for (k = &s[i][*j]; ok && *k && *k != ','; ++k)
        {
            if (!strchr("bcdplfsugtrwx", *k))
            {
                ok = false;
                break;
            }
            else
            {
                ++cnt;
            }
        }
        if (ok && cnt > 0)
        {
            m = (m_arg *)lf_alloc(M_ARG_SIZ);
            for (char *k = &s[i][*j]; *k && *k != ','; ++k)
            {
                switch (*k)
                {
                case 'b':
                    m->b = true;
                    break;
                case 'c':
                    m->c = true;
                    break;
                case 'd':
                    m->d = true;
                    break;
                case 'p':
                    m->f = true;
                    break;
                case 'l':
                    m->l = true;
                    break;
                case 'f':
                    m->r = true;
                    break;
                case 's':
                    m->s = true;
                    break;
                case 'u':
                    m->u = true;
                    break;
                case 'g':
                    m->g = true;
                    break;
                case 't':
                    m->t = true;
                    break;
                case 'r':
                    m->r = true;
                    break;
                case 'w':
                    m->w = true;
                    break;
                case 'x':
                    m->x = true;
                    break;
                default:
                    break;
                }
            }
            *j += cnt;
        }
        else
        {
            if (!ok)
            {
                printf("%s: The option \"m\" doesn't recognize the argument \"%c\".\n", PROGRAM, *k);
            }
            else if (cnt == 0)
            {
                printf(PROGRAM ": The \"m\" option needs an argument after \"=\".\n");
            }
            else
            {
                printf(PROGRAM "Error in the option \"m\".");
            }
            printf("Please try \"" PROGRAM " -h\" for help.\n");
            lf_quit();
        }
    }
    return m;
}

l_arg *set_l_arg(char *s[], int n, int i, int *j)
{
    bool ok = true;
    l_arg *la = NULL;

    if ((*j < strlen(s[i]) - 1) &&
        (s[i][*j + 1] == '='))
    {
        *j += 2;
        int cnt = 0;
        char *k;
        // verify that data are correct
        for (k = &s[i][*j]; ok && *k && *k != ','; ++k)
        {
            if (!strchr("inugspamc", *k))
            {
                ok = false;
                break;
            }
            else
            {
                ++cnt;
            }
        }
        if (ok && cnt > 0)
        {
            la = (l_arg *)lf_alloc(L_ARG_SIZ);
            for (char *k = &s[i][*j]; *k && *k != ','; ++k)
            {
                switch (*k)
                {
                case 'i':
                    la->i = true;
                    break;
                case 'n':
                    la->n = true;
                    break;
                case 'u':
                    la->u = true;
                    break;
                case 'g':
                    la->g = true;
                    break;
                case 's':
                    la->s = true;
                    break;
                case 'p':
                    la->p = true;
                    break;
                case 'a':
                    la->a = true;
                    break;
                case 'm':
                    la->m = true;
                    break;
                case 'c':
                    la->c = true;
                    break;
                default:
                    break;
                }
            }
            *j += cnt;
        }
        else
        {
            if (!ok)
            {
                printf("%s: The option \"l\" doesn't recognize the argument \"%c\".\n", PROGRAM, *k);
            }
            else if (cnt == 0)
            {
                printf(PROGRAM ": The \"l\" option needs an argument after \"=\".\n");
            }
            else
            {
                printf(PROGRAM "Error in the option \"l\".");
            }
            printf("Please try \"" PROGRAM " -h\" for help.\n");
            lf_quit();
        }
    }
    return la;
}

n_arg *set_n_arg(char *s[], int n, int i, int *j)
{
    bool ok = true;
    n_arg *na = NULL;

    if ((*j < strlen(s[i]) - 1) &&
        (s[i][*j + 1] == '='))
    {
        *j += 2;
        int cnt = 0;
        char *k;
        for (k = &s[i][*j]; ok && *k && *k != ','; ++k)
        {
            if (!strchr("bfqs", *k))
            {
                ok = false;
                break;
            }
            else
            {
                ++cnt;
            }
        }
        if (ok && cnt > 0)
        {
            na = (n_arg *)lf_alloc(N_ARG_SIZ);
            for (char *k = &s[i][*j]; *k && *k != ','; ++k)
            {
                switch (*k)
                {
                case 'b':
                    na->b = true;
                    break;
                case 'f':
                    na->f = true;
                    break;
                case 'q':
                    na->q = true;
                    break;
                case 's':
                    na->s = true;
                    break;
                default:
                    break;
                }
            }
            *j += cnt;
        }
        else
        {
            if (!ok)
            {
                printf("%s: The option \"n\" doesn't recognize the argument \"%c\".\n", PROGRAM, *k);
            }
            else if (cnt == 0)
            {
                printf(PROGRAM ": The \"n\" option needs an argument after \"=\".\n");
            }
            else
            {
                printf(PROGRAM "Error in the option \"n\".\n");
            }
            printf("Please try \"" PROGRAM " -h\" for help.\n");
            lf_quit();
        }
    }
    return na;
}

void set_options(int argc, char *argv[], linklist l)
{
    int tmp;
    bool ok = true;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            tmp = strlen(argv[i]);
            for (int j = 1; j < tmp; ++j)
            {
                switch (argv[i][j])
                {
                case ',':
                    continue;
                case '0':
                    LFopt.zero = true;
                    break;
                case '1':
                    LFopt.one = true;
                    break;
                case '2':
                    LFopt.two = true;
                    break;
                case '3':
                    LFopt.three = true;
                    break;
                case 'a':
                    LFopt.a = true;
                    break;
                case 'c':
                    LFopt.c = true;
                    break;
                case 'r':
                    LFopt.r = true;
                    break;
                case 'v':
                    LFopt.v = true;
                    break;
                case 'h':
                    LFopt.h = true;
                    break;
                case 't':
                    LFopt.t = true;
                    LFopt.td = set_t_arg(argv, argc, i, &j);
                    break;
                case 's':
                    LFopt.s = true;
                    LFopt.sc = set_s_arg(argv, argc, i, &j);
                    break;
                case 'm':
                    LFopt.m = true;
                    LFopt.ml = set_m_arg(argv, argc, i, &j);
                    break;
                case 'l':
                    LFopt.l = true;
                    LFopt.ll = set_l_arg(argv, argc, i, &j);
                    break;
                case 'n':
                    LFopt.n = true;
                    LFopt.nl = set_n_arg(argv, argc, i, &j);
                    break;
                default:
                    ok = false;
                    printf("%s: Unknown option \"%c\"\n", PROGRAM, argv[i][j]);
                    break;
                }
            }
        }
        else
        {
            ladd(l, LLAST, argv[i]);
        }
    }
    if (!ok)
    {
        lf_quit();
    }
}

int main(int argc, char *argv[], char *envp[])
{
    linklist l = lopen();

    // init OPT
    memset(&LFopt, 0, OPTIONSIZ);
    set_options(argc, argv, l);
    if (LFopt.h)
    {
        help(0);
    }
    else if (LFopt.v)
    {
        version();
    }
    else
    {
        if (LFopt.t && (LFopt.l + LFopt.zero + LFopt.one + LFopt.two >= true))
        {
            printf(PROGRAM ": The option \"t\" can't be combined with the options \"l\", \"0\", \"1\" and \"2\".\n");
            lf_quit();
        }
        else if (LFopt.l && (LFopt.zero + LFopt.one + LFopt.two >= true))
        {
            if (!LFopt.ll ||
                (LFopt.ll->i + LFopt.ll->n +
                     LFopt.ll->s + LFopt.ll->p +
                     LFopt.ll->u + LFopt.ll->g +
                     LFopt.ll->a + LFopt.ll->m +
                     LFopt.ll->c >
                 true))
            {
                printf(PROGRAM ": The option \"l\" can't be combined with the options \"0\", \"1\" and \"2\", unless \"l\" has only one argument.\n");
                lf_quit();
            }
        }
        else if (LFopt.zero + LFopt.one + LFopt.two > true)
        {
            printf(PROGRAM ": The options \"0\", \"1\" and \"2\" can't be combined..\n");
            lf_quit();
        }

        if (lempty(l))
        {
            ladd(l, LFIRST, "./");
        }
        if (!LFopt.nl)
        {
            LFopt.nl = (n_arg *)lf_alloc(N_ARG_SIZ);
            memset(LFopt.nl, 0, N_ARG_SIZ);
        }
        if (!LFopt.ml)
        {
            LFopt.ml = (m_arg *)lf_alloc(M_ARG_SIZ);
            LFopt.ml->b = true;
            LFopt.ml->c = true;
            LFopt.ml->d = true;
            LFopt.ml->p = true;
            LFopt.ml->l = true;
            LFopt.ml->f = true;
            LFopt.ml->s = true;
            LFopt.ml->u = true;
            LFopt.ml->g = true;
            LFopt.ml->t = true;
            LFopt.ml->r = true;
            LFopt.ml->w = true;
            LFopt.ml->x = true;
        }
        if (LFopt.l)
        {
            if (LFopt.ll)
            {

                // follow link if long format
                if (LFopt.ll->i + LFopt.ll->n +
                        LFopt.ll->s + LFopt.ll->p +
                        LFopt.ll->u + LFopt.ll->g +
                        LFopt.ll->a + LFopt.ll->m +
                        LFopt.ll->c ==
                    true)
                {
                    LFopt.nl->f = false;
                }
                else
                {
                    LFopt.nl->f = true;
                }
            }
            else
            {
                LFopt.nl->f = true;
                LFopt.ll = (l_arg *)lf_alloc(L_ARG_SIZ);
                LFopt.ll->i = true;
                LFopt.ll->n = true;
                LFopt.ll->u = true;
                LFopt.ll->g = true;
                LFopt.ll->p = true;
                LFopt.ll->s = true;
                LFopt.ll->m = true;
            }
        }
        if (LFopt.t)
        {
            LFopt.nl->f = true;
        }
        if (LFopt.c)
        {
            LFcolorlist = scan_for_color();
            if (lempty(LFcolorlist))
            {
                printf(PROGRAM ": warning: \"-c\" not available because the \"LS_COLORS\" environment variable is not set.\n");
                LFopt.c = false;
            }
            else if (!getcolor(LFcolorlist, "rs", false))
            { // at least LS_COLORS must have value for "rs"
                printf(PROGRAM ": warning: \"-c\" not available because the environment variable \"LS_COLORS\" has no value \"rs\".\n");
                LFopt.c = false;
            }
        }
        run(l);
    }
    lf_quit(l);
    lclose(l);
    return 0;
}