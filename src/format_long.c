#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "format_column.h"
#include "format_long.h"
#include "format_list.h"
#include "display.h"
#include "common.h"

void long_print(char *nm, int m, _bool rtl)
{
    // rtl: left to right
    int x = m - strlen(nm);

    if (rtl)
    {
        while (x)
        {
            printf(" ");
            --x;
        }
    }
    printf("%s ", nm);
    if (!rtl)
    {
        while (x)
        {
            printf(" ");
            --x;
        }
    }
}

void long_display(linklist l, _file_info *files_infos)
{
    _file file;
    int i = 0;

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;
        // permissions
        if (_opt.ll->p)
        {
            long_print(files_infos->p[i], files_infos->mp, _false);
        }
        // size
        if (_opt.ll->s)
        {
            long_print(files_infos->s[i], files_infos->ms, _true);
        }
        // inodes
        if (_opt.ll->i)
        {
            long_print(files_infos->i[i], files_infos->mi, _false);
        }
        // nlink
        if (_opt.ll->n)
        {
            long_print(files_infos->n[i], files_infos->mn, _false);
        }
        // user
        if (_opt.ll->u)
        {
            long_print(files_infos->u[i], files_infos->mu, _false);
        }
        // group
        if (_opt.ll->g)
        {
            long_print(files_infos->g[i], files_infos->mg, _false);
        }
        // access
        if (_opt.ll->a)
        {
            long_print(files_infos->a[i], files_infos->ma, _false);
        }
        // modification
        if (_opt.ll->m)
        {
            long_print(files_infos->m[i], files_infos->mm, _false);
        }
        // change
        if (_opt.ll->c)
        {
            long_print(files_infos->c[i], files_infos->mc, _false);
        }
        // file's name
        display(file->name, &file->st.st_mode, _true);
    }
}

void _free_tb(char **tb, int n)
{
    if (tb)
    {
        char **tmp = tb;
        while (n)
        {
            if (tmp[0][0] != '?')
            {
                free(*tmp);
                ++tmp;
            }
            --n;
        }
        free(tb);
    }
}
void long_main(linklist l)
{
    _file_info files_infos;
    _file file;
    iterator it;
    int i = 0, tmp;
    _bool onecol = _false; // multiple column
    char **col = NULL;
    int siz = sizeof(char *) * l->count;
    char *novalue = "?";

    onecol = (_opt.ll->i + _opt.ll->n + _opt.ll->s + _opt.ll->r +
                  _opt.ll->p + _opt.ll->u + _opt.ll->g +
                  _opt.ll->a + _opt.ll->m + _opt.ll->c ==
              _true);

    // set default maximums sizes.
    memset(&files_infos, 0, _FILE_INFO_SIZE);
    files_infos.ma = 1;
    files_infos.mc = 1;
    files_infos.mg = 1;
    files_infos.mi = 1;
    files_infos.mm = 1;
    files_infos.mn = 1;
    files_infos.mp = 1;
    files_infos.ms = 1;
    files_infos.mu = 1;

    if (_opt.ll->i)
    {
        col = files_infos.i = (char **)_alloc(siz);
    }
    if (_opt.ll->n)
    {
        col = files_infos.n = (char **)_alloc(siz);
    }
    if (_opt.ll->s || _opt.ll->r)
    {
        col = files_infos.s = (char **)_alloc(siz);
    }
    if (_opt.ll->p)
    {
        col = files_infos.p = (char **)_alloc(siz);
    }
    if (_opt.ll->u)
    {
        col = files_infos.u = (char **)_alloc(siz);
    }
    if (_opt.ll->g)
    {
        col = files_infos.g = (char **)_alloc(siz);
    }
    if (_opt.ll->m)
    {
        col = files_infos.m = (char **)_alloc(siz);
    }
    if (_opt.ll->a)
    {
        col = files_infos.a = (char **)_alloc(siz);
    }
    if (_opt.ll->c)
    {
        col = files_infos.c = (char **)_alloc(siz);
    }

    for (it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;
        // inodes
        if (_opt.ll->i)
        {
            if (file->err)
            {
                files_infos.i[i] = novalue;
            }
            else
            {
                files_infos.i[i] = long_ino(NULL, file->st.st_ino);
                tmp = strlen(files_infos.i[i]);
                if (tmp > files_infos.mi)
                {
                    files_infos.mi = tmp;
                }
            }
        }
        // nlink
        if (_opt.ll->n)
        {
            if (file->err)
            {
                files_infos.n[i] = novalue;
            }
            else
            {
                files_infos.n[i] = long_nlink(NULL, file->st.st_nlink);
                tmp = strlen(files_infos.n[i]);
                if (tmp > files_infos.mn)
                {
                    files_infos.mn = tmp;
                }
            }
        }
        // user
        if (_opt.ll->u)
        {
            if (file->err)
            {
                files_infos.u[i] = novalue;
            }
            else
            {
                files_infos.u[i] = long_user(NULL, file->st.st_uid);
                tmp = strlen(files_infos.u[i]);
                if (tmp > files_infos.mu)
                {
                    files_infos.mu = tmp;
                }
            }
        }
        // group
        if (_opt.ll->g)
        {
            if (file->err)
            {
                files_infos.g[i] = novalue;
            }
            else
            {
                files_infos.g[i] = long_group(NULL, file->st.st_gid);
                tmp = strlen(files_infos.g[i]);
                if (tmp > files_infos.mg)
                {
                    files_infos.mg = tmp;
                }
            }
        }
        // size
        if (_opt.ll->s)
        {
            if (file->err)
            {
                files_infos.s[i] = novalue;
            }
            else
            {
                files_infos.s[i] = long_size(NULL, file->st.st_size);
                tmp = strlen(files_infos.s[i]);
                if (tmp > files_infos.ms)
                {
                    files_infos.ms = tmp;
                }
            }
        }
        // readable size
        else if (_opt.ll->r)
        {
            if (file->err)
            {
                files_infos.s[i] = novalue;
            }
            else
            {
                files_infos.s[i] = long_size_readable(NULL, file->st.st_size);
                tmp = strlen(files_infos.s[i]);
                if (tmp > files_infos.ms)
                {
                    files_infos.ms = tmp;
                }
            }
        }
        // permissions
        if (_opt.ll->p)
        {
            if (file->err)
            {
                files_infos.p[i] = novalue;
            }
            else
            {
                files_infos.p[i] = long_permission(NULL, &file->st.st_mode);
                tmp = strlen(files_infos.p[i]);
                if (tmp > files_infos.mp)
                {
                    files_infos.mp = tmp;
                }
            }
        }
        if (_opt.ll->m)
        { // modification time; max_mtime = 16 (doesn't change)
            if (file->err)
            {
                files_infos.m[i] = novalue;
            }
            else
            {
                files_infos.m[i] = long_time(NULL, &file->st.st_mtime);
                tmp = strlen(files_infos.m[i]);
                if (tmp > files_infos.mm)
                {
                    files_infos.mm = tmp;
                }
            }
        }
        if (_opt.ll->a)
        { // modification time; max_mtime = 16 (doesn't change)
            if (file->err)
            {
                files_infos.a[i] = novalue;
            }
            else
            {
                files_infos.a[i] = long_time(NULL, &file->st.st_atime);
                tmp = strlen(files_infos.a[i]);
                if (tmp > files_infos.ma)
                {
                    files_infos.ma = tmp;
                }
            }
        }
        if (_opt.ll->c)
        { // modification time; max_mtime = 16 (doesn't change)
            if (file->err)
            {
                files_infos.c[i] = novalue;
            }
            else
            {
                files_infos.c[i] = long_time(NULL, &file->st.st_ctime);
                tmp = strlen(files_infos.c[i]);
                if (tmp > files_infos.mc)
                {
                    files_infos.mc = tmp;
                }
            }
        }
    }

    if (onecol)
    { // display multiple columns
        if (_opt._2 || _opt._1 || _opt._3 || _opt._4)
        {
            list_main(l, col);
        }
        else
        {
            column_main(l, col);
        }
    }
    else
    { // display one column
        long_display(l, &files_infos);
    }
    _free_tb(files_infos.i, l->count);
    _free_tb(files_infos.n, l->count);
    _free_tb(files_infos.u, l->count);
    _free_tb(files_infos.g, l->count);
    _free_tb(files_infos.s, l->count);
    _free_tb(files_infos.p, l->count);
    _free_tb(files_infos.m, l->count);
    _free_tb(files_infos.a, l->count);
    _free_tb(files_infos.c, l->count);
}

int number_size(long int n)
{
    int i = 1;
    while (n != 0)
    {
        n /= 10;
        ++i;
    }
    return i;
}

char *long_ino(char *buf, ino_t data)
{
    buf = (char *)_alloc(sizeof(char) * number_size(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_nlink(char *buf, nlink_t data)
{
    buf = (char *)_alloc(sizeof(char) * number_size(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_size_readable(char *buf, long int data)
{
    double cnt = data;
    int i = 0;
    char units[10] = {'B', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', '?'};

    if (!buf)
    {
        buf = (char *)_alloc(sizeof(char) * number_size(data));
    }
    while (cnt > 1024)
    {
        cnt /= 1024;
        i++;
    }
    if (cnt - (int)cnt < 0.1)
    {
        sprintf(buf, "%d%c", (int)cnt, units[i]);
    }
    else if (cnt - (int)cnt > 0.9)
    {
        sprintf(buf, "%d%c", (int)cnt + 1, units[i]);
    }
    else
    {
        sprintf(buf, "%.1f%c", cnt, units[i]);
    }

    return buf;
}
char *long_size(char *buf, long int data)
{
    if (!buf)
    {
        buf = (char *)_alloc(sizeof(char) * number_size(data));
    }
    sprintf(buf, "%ld", data);

    return buf;
}

char *long_user(char *user, uid_t uid)
{
    struct passwd *pw;

    pw = getpwuid(uid);
    if (!pw)
    {
        if (!user)
        {
            user = (char *)_alloc(sizeof(char) * 2);
        }
        user[0] = '?';
        user[1] = 0;
        return user;
    }
    if (!user)
    {
        user = (char *)_alloc(sizeof(char) * (strlen(pw->pw_name) + 1));
    }
    strcpy(user, pw->pw_name);
    return user;
}

char *long_group(char *group, gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    if (!grp)
    {
        if (!group)
        {
            group = (char *)_alloc(sizeof(char) * 2);
        }
        group[0] = '?';
        group[1] = 0;
        return group;
    }
    if (!group)
    {
        group = (char *)_alloc(sizeof(char) * (strlen(grp->gr_name) + 1));
    }
    strcpy(group, grp->gr_name);

    return group;
}

char *long_time(char *buf, const time_t *atm)
{
    struct tm *t;
    int bufflen = 100;
    int repeat = 12, result;

    char *tmp = (char *)malloc(sizeof(char) * bufflen);
    t = localtime(atm);
    while (!(result = strftime(tmp, bufflen, _time_style, t)) && (tmp[0] == 0) && repeat)
    {
        --repeat;
        bufflen += 100;
        tmp = realloc(tmp, bufflen);
    }
    if (repeat == 0 && result == 0)
    {
        _quit("%s: error: can't allocate enough memory to handle the time style");
    }

    if (!buf)
    {
        buf = (char *)realloc(tmp, strlen(tmp) + 1);
    }
    return buf;
}

char *long_permission(char *b, __mode_t *m)
{
    int i = 0;
    mode_t modes[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    if (!b)
    {
        b = (char *)_alloc(sizeof(char) * 12);
    }
    b[0] = file_type(m);
    for (i = 1; i < 10; i += 3)
    {
        if (*m & modes[i - 1])
        {
            b[i] = 'r';
        }
        else
        {
            b[i] = '-';
        }
        if (*m & modes[i])
        {
            b[i + 1] = 'w';
        }
        else
        {
            b[i + 1] = '-';
        }
        if (*m & modes[i + 1])
        {
            b[i + 2] = 'x';
        }
        else
        {
            b[i + 2] = '-';
        }
    }
    if (*m & S_ISUID)
    {
        if (b[3] == '-')
        {
            b[3] = 'S';
        }
        else
        {
            b[3] = 's';
        }
    }
    if (*m & S_ISGID)
    {
        if (b[6] == '-')
        {
            b[6] = 'S';
        }
        else
        {
            b[6] = 's';
        }
    }
    if (*m & S_ISVTX)
    {
        b[10] = 't';
        b[11] = 0;
    }
    else
    {
        b[10] = 0;
    }

    return b;
}
