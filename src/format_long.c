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

#define novalue "?"

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
    if (!nm)
    {
        nm = novalue;
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
        // permissions
        if (_opt.ll->p)
        {
            long_print(files_infos->p[i], files_infos->mp, _false);
        }
        // size
        if (_opt.ll->s || _opt.ll->r)
        {
            long_print(files_infos->s[i], files_infos->ms, _true);
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
    _file_info files_info;
    _file file;
    iterator it;
    int i = 0, tmp;
    _bool onecol = _false; // multiple column
    char **col = NULL;
    int siz = sizeof(char *) * l->count;

    onecol = (_opt.ll->i + _opt.ll->n + _opt.ll->s + _opt.ll->r +
                  _opt.ll->p + _opt.ll->u + _opt.ll->g +
                  _opt.ll->a + _opt.ll->m + _opt.ll->c ==
              _true);

    // set default maximums sizes.
    memset(&files_info, 0, _FILE_INFO_SIZE);
    files_info.ma = 1;
    files_info.mc = 1;
    files_info.mg = 1;
    files_info.mi = 1;
    files_info.mm = 1;
    files_info.mn = 1;
    files_info.mp = 1;
    files_info.ms = 1;
    files_info.mu = 1;

    if (_opt.ll->i)
    {
        col = files_info.i = (char **)_alloc(siz);
    }
    if (_opt.ll->n)
    {
        col = files_info.n = (char **)_alloc(siz);
    }
    if (_opt.ll->s || _opt.ll->r)
    {
        col = files_info.s = (char **)_alloc(siz);
    }
    if (_opt.ll->p)
    {
        col = files_info.p = (char **)_alloc(siz);
    }
    if (_opt.ll->u)
    {
        col = files_info.u = (char **)_alloc(siz);
    }
    if (_opt.ll->g)
    {
        col = files_info.g = (char **)_alloc(siz);
    }
    if (_opt.ll->m)
    {
        col = files_info.m = (char **)_alloc(siz);
    }
    if (_opt.ll->a)
    {
        col = files_info.a = (char **)_alloc(siz);
    }
    if (_opt.ll->c)
    {
        col = files_info.c = (char **)_alloc(siz);
    }

    for (it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;
        // inodes
        if (_opt.ll->i)
        {
            files_info.i[i] = long_ino(NULL, file->st.st_ino);
            tmp = strlen(files_info.i[i]);
            if (tmp > files_info.mi)
            {
                files_info.mi = tmp;
            }
        }
        // nlink
        if (_opt.ll->n)
        {
            files_info.n[i] = long_nlink(NULL, file->st.st_nlink);
            tmp = strlen(files_info.n[i]);
            if (tmp > files_info.mn)
            {
                files_info.mn = tmp;
            }
        }
        // user
        if (_opt.ll->u)
        {
            files_info.u[i] = long_user(NULL, file->st.st_uid);
            tmp = strlen(files_info.u[i]);
            if (tmp > files_info.mu)
            {
                files_info.mu = tmp;
            }
        }
        // group
        if (_opt.ll->g)
        {
            files_info.g[i] = long_group(NULL, file->st.st_gid);
            tmp = strlen(files_info.g[i]);
            if (tmp > files_info.mg)
            {
                files_info.mg = tmp;
            }
        }
        // size and readable size
        if (_opt.ll->r || _opt.ll->s)
        {
            if (_opt.ll->r)
            {
                files_info.s[i] = long_size_readable(NULL, file->st.st_size);
            }
            else
            {
                files_info.s[i] = long_size(NULL, file->st.st_size);
            }
            tmp = strlen(files_info.s[i]);
            if (tmp > files_info.ms)
            {
                files_info.ms = tmp;
            }
        }
        // permissions
        if (_opt.ll->p)
        {
            files_info.p[i] = long_permission(NULL, &file->st.st_mode);
            tmp = strlen(files_info.p[i]);
            if (tmp > files_info.mp)
            {
                files_info.mp = tmp;
            }
        }
        if (_opt.ll->m)
        { // modification time; max_mtime = 16

            files_info.m[i] = long_time(NULL, &file->st.st_mtime);
            tmp = strlen(files_info.m[i]);
            if (tmp > files_info.mm)
            {
                files_info.mm = tmp;
            }
        }
        if (_opt.ll->a)
        { // modification time; max_mtime = 16

            files_info.a[i] = long_time(NULL, &file->st.st_atime);
            tmp = strlen(files_info.a[i]);
            if (tmp > files_info.ma)
            {
                files_info.ma = tmp;
            }
        }
        if (_opt.ll->c)
        { // modification time; max_mtime = 16

            files_info.c[i] = long_time(NULL, &file->st.st_ctime);
            tmp = strlen(files_info.c[i]);
            if (tmp > files_info.mc)
            {
                files_info.mc = tmp;
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
        long_display(l, &files_info);
    }
    _free_tb(files_info.i, l->count);
    _free_tb(files_info.n, l->count);
    _free_tb(files_info.u, l->count);
    _free_tb(files_info.g, l->count);
    _free_tb(files_info.s, l->count);
    _free_tb(files_info.p, l->count);
    _free_tb(files_info.m, l->count);
    _free_tb(files_info.a, l->count);
    _free_tb(files_info.c, l->count);
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
    buf = (char *)_alloc(number_size(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_nlink(char *buf, nlink_t data)
{
    buf = (char *)_alloc(number_size(data));
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
        buf = (char *)_alloc(number_size(data));
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
        buf = (char *)_alloc(number_size(data));
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
            user = (char *)_alloc(2);
        }
        user[0] = '?';
        user[1] = 0;
        return user;
    }
    if (!user)
    {
        user = (char *)_alloc((strlen(pw->pw_name) + 1));
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
            group = (char *)_alloc(2);
        }
        group[0] = '?';
        group[1] = 0;
        return group;
    }
    if (!group)
    {
        group = (char *)_alloc((strlen(grp->gr_name) + 1));
    }
    strcpy(group, grp->gr_name);

    return group;
}

char *long_time(char *buf, const time_t *atm)
{
    struct tm *t;
    int buflen = 100;
    int repeat = 12, result;

    char *tmp = (char *)malloc(buflen);
    t = localtime(atm);
    while (!(result = strftime(tmp, buflen, _time_style, t)) && (tmp[0] == 0) && repeat)
    {
        --repeat;
        buflen += 100;
        tmp = realloc(tmp, buflen);
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
        b = (char *)_alloc(12);
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
