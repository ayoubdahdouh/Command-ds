#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "list.h"
#include "common.h"

int set_t_arg(char **c)
{
    int depth = 0;
    _bool ok = _true;
    int cnt = 0;

    if (**c && **c == '=')
    {
        ++*c;
        for (char *d = *c; ok && *d; ++d)
        {
            if (*d >= '0' && *d <= '9')
            {
                ++cnt;
            }
            else
            {
                ok = _false;
                break;
            }
        }
        if (ok && cnt)
        {
            char *ss = (char *)_alloc((cnt + 1) * sizeof(char));
            strncpy(ss, *c, cnt);
            sscanf(ss, "%d", &depth);
            free(ss);
            *c += cnt - 1;
        }
        else if (cnt == 0)
        {
            _quit(PROGRAM ": The \"s\" option needs an argument after \"=\".");
        }
        else if (!ok)
        {
            _quit(PROGRAM ": For the \"t\" option, the argument must be a number.");
        }
    }
    return (depth == 0) ? MAX_DEPTH : depth;
}

char set_s_arg(char **c)
{
    if (**c && **c == '=')
    {
        ++*c;
        if (**c)
        {
            if (strchr("dinsugamcte", **c))
            {
                if (*(*c + 1))
                {
                    _quit(PROGRAM ": The \"s\" option accepts only one argument.");
                }
            }
            else
            {
                printf("%s: The option \"s\" doesn't recognize the argument \"%c\".", PROGRAM, **c);
                _quit(NULL);
            }
        }
        else
        {
            _quit(PROGRAM ": The \"s\" option needs an argument after \"=\".");
        }
    }
    return **c;
}

_arg_m *set_m_arg(char **c)
{
    int cnt = 0;
    _bool ok = _true;
    _arg_m *argm = NULL;
    char *d;

    if (**c && **c == '=')
    {
        ++*c;
        argm = (_arg_m *)_alloc(_ARG_M_SIZE);
        memset(argm, 0, _ARG_M_SIZE);
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'h':
                argm->h = _true;
                break;
            case 'b':
                argm->b = _true;
                break;
            case 'c':
                argm->c = _true;
                break;
            case 'd':
                argm->d = _true;
                break;
            case 'p':
                argm->p = _true;
                break;
            case 'l':
                argm->l = _true;
                break;
            case 'f':
                argm->r = _true;
                break;
            case 's':
                argm->s = _true;
                break;
            case 'r':
                argm->r = _true;
                break;
            case 'u':
                argm->u = _true;
                break;
            case 'g':
                argm->g = _true;
                break;
            case 't':
                argm->t = _true;
                break;
            case '1':
                argm->_1 = _true;
                break;
            case '2':
                argm->_2 = _true;
                break;
            case '3':
                argm->_3 = _true;
                break;
            case '4':
                argm->_4 = _true;
                break;
            case '5':
                argm->_5 = _true;
                break;
            case '6':
                argm->_6 = _true;
                break;
            case '7':
                argm->_7 = _true;
                break;
            case '8':
                argm->_8 = _true;
                break;
            case '9':
                argm->_9 = _true;
                break;
            default:
                ok = _false;
                --cnt;
                break;
            }
            ++cnt;
        }
        if (ok && cnt)
        {
            *c += cnt - 1;
        }
        else if (cnt == 0)
        {
            free(argm);
            _quit(PROGRAM ": The option \"m\" needs an argument after \"=\".");
        }
        else
        { // if (!ok)
            printf("%s: The option \"m\" doesn't recognize the argument \"%c\".\n", PROGRAM, *d);
            free(argm);
            _quit(NULL);
        }
    }
    return argm;
}

_arg_l *set_l_arg(char **c)
{
    _bool ok = _true;
    _arg_l *argl = NULL;
    int cnt = 0;
    char *d;

    if (**c && **c == '=')
    {
        ++*c;
        argl = (_arg_l *)_alloc(_ARG_L_SIZE);
        memset(argl, 0, _ARG_L_SIZE);
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'i':
                argl->i = _true;
                break;
            case 'n':
                argl->n = _true;
                break;
            case 'p':
                argl->p = _true;
                break;
            case 's':
                argl->s = _true;
                break;
            case 'r':
                argl->r = _true;
                break;
            case 'u':
                argl->u = _true;
                break;
            case 'g':
                argl->g = _true;
                break;
            case 'a':
                argl->a = _true;
                break;
            case 'm':
                argl->m = _true;
                break;
            case 'c':
                argl->c = _true;
                break;
            default:
                ok = _false;
                --cnt;
                break;
            }
            ++cnt;
        }
        if (ok && cnt)
        {
            *c += cnt - 1;
        }
        else if (cnt == 0)
        {
            free(argl);
            _quit(PROGRAM ": The \"l\" option needs an argument after \"=\".\n");
        }
        else
        { // if (!ok)
            printf("%s: The option \"l\" doesn't recognize the argument \"%c\".\n", PROGRAM, *d);
            free(argl);
            _quit(NULL);
        }
    }
    return argl;
}

_arg_n *set_n_arg(char **c)
{
    _bool ok = _true;
    _arg_n *argn = NULL;
    int cnt = 0;
    char *d;

    if (**c && **c == '=')
    {
        ++*c;
        argn = (_arg_n *)_alloc(_ARG_N_SIZE);
        memset(argn, 0, _ARG_N_SIZE);
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'c':
                argn->c = _true;
                break;
            case 'b':
                argn->b = _true;
                break;
            case 'f':
                argn->f = _true;
                break;
            case 'q':
                argn->q = _true;
                break;
            case 'i':
                argn->i = _true;
                break;
            default:
                ok = _false;
                --cnt;
                break;
            }
            ++cnt;
        }
        if (ok && cnt > 0)
        {
            *c += cnt - 1;
        }
        else if (!ok)
        {
            printf("%s: The option \"n\" doesn't recognize the argument \"%c\".\n", PROGRAM, *d);
            free(argn);
            _quit(NULL);
        }
        else if (cnt == 0)
        {
            free(argn);
            _quit(PROGRAM ": The option \"n\" needs an argument after \"=\".");
        }
    }
    return argn;
}
char *set_f_arg(char **c)
{
    char default_style[6] = "%F %R";
    char *time_style = NULL;
    if (**c && (**c == '=') && *(*c + 1))
    {
        ++*c;
        int cnt = strlen(*c);
        time_style = (char *)_alloc((cnt + 1) * sizeof(char));
        strncpy(time_style, *c, cnt);
        *c += cnt - 1;
    }
    else
    {
        time_style = (char *)_alloc(6 * sizeof(char));
        strcpy(time_style, default_style);
    }

    return time_style;
}

void set_options(int argc, char *argv[], linklist l)
{
    _bool ok = _true;
    char *c;

    memset(&_opt, 0, _OPTIONS_SIZE);
    --argc;
    while (ok && argc)
    {
        c = argv[argc];
        if (*c == '-')
        {
            ++c;
            for (; ok && *c; ++c)
            {
                switch (*c)
                {
                case '1':
                    _opt._1 = _true;
                    break;
                case '2':
                    _opt._2 = _true;
                    break;
                case '3':
                    _opt._3 = _true;
                    break;
                case '4':
                    _opt._4 = _true;
                    break;
                case 'd':
                    _opt.d = _true;
                    break;
                case 'v':
                    _opt.v = _true;
                    break;
                case 'h':
                    _opt.h = _true;
                    break;
                case 't':
                    ++c;
                    _opt.t = _true;
                    _opt.td = set_t_arg(&c);
                    if (!_opt.td)
                    {
                        --c;
                    }
                    break;
                case 's':
                    ++c;
                    _opt.s_char = set_s_arg(&c);
                    if (!_opt.s_char)
                    {
                        --c;
                    }
                    break;
                case 'm':
                    ++c;
                    _opt.m = _true;
                    _opt.ml = set_m_arg(&c);
                    if (!_opt.ml)
                    {
                        --c;
                    }
                    break;
                case 'l':
                    ++c;
                    _opt.l = _true;
                    _opt.ll = set_l_arg(&c);
                    if (!_opt.ll)
                    {
                        --c;
                    }
                    break;
                case 'n':
                    ++c;
                    _opt.n = _true;
                    _opt.nl = set_n_arg(&c);
                    if (!_opt.nl)
                    {
                        --c;
                    }
                    break;
                case 'f':
                    ++c;
                    _time_style = set_f_arg(&c);
                    break;
                default:
                    --c;
                    ok = _false;
                    break;
                }
            }
        }
        else
        {
            // it's a file/folder.
            ladd(l, LFIRST, c);
        }
        --argc;
    }
    if (!ok)
    {
        printf("%s: Unknown option \"%c\"\n", PROGRAM, *c);
        _quit(NULL);
    }
}
