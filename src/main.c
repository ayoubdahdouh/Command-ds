#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <locale.h>
#include <errno.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"
#include "args.h"
#include "display.h"
#include "format_long.h"

_options _opt;
char *_path = NULL;
char *_buffer = NULL;
int _path_len = 0;
linklist _colors_list = NULL;
char *_time_style = NULL;

void run(linklist l)
{
    char *nm;
    int len;
    struct stat s;
    _tree_info tree;
    _file file;
    linklist files_list = lopen();
    long int mularg = (l->count == 1) ? 0 : l->count;

    _initial();

    if (_opt.t)
    {
        tree.level = 0;
        tree.has_next = (char *)_alloc(PATH_MAX);
    }
    for (iterator it = lat(l, LFIRST); it; linc(&it))
    {
        nm = (char *)it->data;
        len = strlen(nm);
        if (len != 1 && nm[len - 1] == '/')
        {
            nm[--len] = 0;
        }
        // if multiple arguments then print name of each argument before list.
        if (mularg)
        {
            printf("%s:\n", nm);
            --mularg;
        }
        if (_stat(nm, &s))
        {
            if ((S_ISDIR(s.st_mode) || S_ISLNK(s.st_mode)) && !_opt.d)
            {
                if (S_ISDIR(s.st_mode))
                {
                    strcpy(_path, nm);
                    _path_len = len;
                }
                else
                {
                    if (!_link(nm))
                    {
                        printf("%s: Cannot read the reference of \"%s\": %s\n", PROGRAM, nm, strerror(errno));
                        continue;
                    }
                    strcpy(_path, _buffer);
                    len = strlen(_buffer);
                    if (len != 1 && nm[len - 1] == '/')
                    {
                        nm[--len] = 0;
                    }
                    _path_len = len;
                    if (!_stat(_buffer, &s))
                    {
                        printf("%s: Cannot access the reference of \"%s\": %s\n", PROGRAM, nm, strerror(errno));
                        continue;
                    }
                }
                if (_opt.t)
                {
                    display(_path, &s.st_mode, _true);
                }
                if (_path[_path_len - 1] != '/')
                {
                    _path[_path_len++] = '/';
                    _path[_path_len] = 0;
                }
                core(&tree);
            }
            else
            {
                if (_opt.l)
                {
                    lreset(files_list);
                    file = (_file)_alloc(_FILE_SIZE);
                    file->name = nm;
                    file->st = s;
                    ladd(files_list, LFIRST, file);
                    long_main(files_list);
                }
                else
                {
                    display(nm, &s.st_mode, _true);
                }
            }
            if (mularg)
            {
                printf("\n");
            }
        }
        else
        {
            printf("%s: \"%s\": %s\n", PROGRAM, nm, strerror(errno));
        }
    }
    lclose(files_list);
}

int main(int argc, char *argv[], char *envp[])
{
    linklist l = lopen();
    _bool onecol;

    setlocale(LC_ALL, "");

    set_options(argc, argv, l);

    if (_opt.h)
    {
        _print_help();
        exit(EXIT_SUCCESS);
    }
    if (_opt.v)
    {
        _print_version();
        exit(EXIT_SUCCESS);
    }
    if (_opt.l)
    {
        if (!_opt.ll)
        {
            _opt.ll = (_arg_l *)_alloc(_ARG_L_SIZE);
            memset(_opt.ll, 0, _ARG_L_SIZE);
            _opt.ll->n = _true;
            _opt.ll->p = _true;
            _opt.ll->r = _true;
            _opt.ll->u = _true;
            _opt.ll->g = _true;
            _opt.ll->m = _true;
        }
        else
        {
            if (_opt.ll->r && _opt.ll->s)
            {
                _opt.ll->s = _false;
            }
        }
    }
    onecol = (_opt.ll &&
              (_opt.ll->i + _opt.ll->n + _opt.ll->s + _opt.ll->r +
                   _opt.ll->p + _opt.ll->u + _opt.ll->g + _opt.ll->a +
                   _opt.ll->m + _opt.ll->c ==
               _true));

    if (_opt.t && (_opt.l + _opt._1 + _opt._2 + _opt._3 + _opt._4 >= _true))
    {
        _quit(PROGRAM ": The option \"t\" can't be combined with the options \"i\", \"1\", \"2\", \"3\" and \"4\".");
    }
    if (_opt.l && (_opt._1 + _opt._2 + _opt._3 + _opt._4 >= _true) && !onecol)
    {
        _quit(PROGRAM ": The option \"i\" can't be combined with the options \"1\", \"2\", \"3\" and \"4\", unless \"i\" has only one argument.");
    }
    if (_opt._1 + _opt._2 + _opt._3 + _opt._4 > _true)
    {
        _quit(PROGRAM ": The options \"1\", \"2\", \"3\" and \"4\" can't be combined.");
    }
    if (_opt.t && _opt.td > MAX_DEPTH)
    {
        printf("%s: maximum depth is %d\n", PROGRAM, MAX_DEPTH);
        _quit(NULL);
    }
    if (!_opt.ml)
    {
        _opt.ml = (_arg_m *)_alloc(_ARG_M_SIZE);
        if (_opt.m)
        {
            _opt.ml->h = _true;
        }
        _opt.ml->b = _true;
        _opt.ml->c = _true;
        _opt.ml->d = _true;
        _opt.ml->l = _true;
        _opt.ml->p = _true;
        _opt.ml->r = _true;
        _opt.ml->s = _true;
    }
    else
    {
        if (!_opt.ml->d &&
            !_opt.ml->r &&
            !_opt.ml->b &&
            !_opt.ml->c &&
            !_opt.ml->l &&
            !_opt.ml->p &&
            !_opt.ml->s)
        {
            _opt.ml->b = _true;
            _opt.ml->c = _true;
            _opt.ml->d = _true;
            _opt.ml->l = _true;
            _opt.ml->p = _true;
            _opt.ml->r = _true;
            _opt.ml->s = _true;
        }
    }

    if (!_opt.nl)
    {
        _opt.nl = (_arg_n *)_alloc(_ARG_N_SIZE);
        // set all arguments to _false
        memset(_opt.nl, 0, _ARG_N_SIZE);
        if (_opt.n)
        { // by default n without argument
            // follow links + color names + quotes spaced names
            if ((_opt.l && !onecol) || _opt.t)
            {
                _opt.nl->f = _true;
            }
            _opt.nl->c = _true;
            _opt.nl->q = _true;
        }
    }
    else
    {
        // disable backslash if quoting names is enabled.
        if (_opt.nl->b && _opt.nl->q)
        {
            _opt.nl->b = _false;
        }
    }

    if (_opt.nl->c)
    {
        _colors_list = scan_for_colors();
        if (lempty(_colors_list))
        {
            printf(PROGRAM ": warning: \"-c\" needs the environment variable \"LS_COLORS\".\n");
            _opt.nl->c = _false;
        }
        else if (!getcolor(_colors_list, "rs", _false))
        { // LS_COLORS must have at least "rs" value
            printf(PROGRAM ": warning: \"-c\" unavailable due to the environment variable \"LS_COLORS\" has no value \"rs\".\n");
            _opt.nl->c = _false;
        }
    }
    if (!_time_style)
    {
        _time_style = "%F %R";
    }

    // default folder to list is current working directory.
    if (lempty(l))
    {
        ladd(l, LFIRST, ".");
    }
    run(l);
    _quit(NULL);
    lclose(l);
    return 0;
}