#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "format_long.h"
#include "common.h"
#include "format_column.h"
#include "display.h"

void long_print(char *fn, int m, int f)
{ // flag  is used to adjust the data (suffix/prefix)
    int x = m - strlen(fn);

    if (f)
    {
        for (int i = 0; i < x; i++)
        {
            printf(" ");
        }
    }
    printf("%s ", fn);
    if (!f)
    {
        for (int i = 0; i < x; i++)
        {
            printf(" ");
        }
    }
}

void long_display(linklist l, format_long_t *fl, int mp, int ms, int mu, int mg, int mi, int mn)
{
    char buf[20];
    lftype t;
    int j = 0;
    for (iterator i = lat(l, LFIRST); i; linc(&i))
    {
        t = (lftype)i->data;
        if (LF_opt.l || LF_opt.i)
        {
            long_print(fl->i[j], mi, false);
        }
        if (LF_opt.l || LF_opt.n)
        {
            long_print(fl->n[j], mn, false);
        }
        if (LF_opt.l || LF_opt.p)
        {
            long_print(fl->p[j], mp, false);
        }
        if (LF_opt.l || LF_opt.u)
        {
            long_print(fl->u[j], mu, false);
        }
        if (LF_opt.l || LF_opt.g)
        {
            long_print(fl->g[j], mg, false);
        }
        if (LF_opt.l || LF_opt.m)
        {
            long_mtime(buf, &t->st.st_mtime);
            printf("%s ", buf);
        }
        if (LF_opt.l || LF_opt.s)
        {
            long_print(fl->s[j], ms, 1);
        }
        lfprint(t->name, &t->st.st_mode, true, true);
        j++;
    }
}

void long_main(linklist l)
{
    format_long_t fl;
    lftype t;
    int mu = 0, mg = 0, ms = 0, mp = 0, mi = 0, mn = 0;
    int j = 0, tmp;
    bool mul_cl = false;
    char **cl = NULL;
    iterator it;
    int siz = sizeof(char *) * l->count;

    if (!LF_opt.l && (LF_opt.s ^ LF_opt.p ^ LF_opt.m ^ LF_opt.u ^ LF_opt.g ^ LF_opt.i ^ LF_opt.n))
    { // if only one of the options s, p, m, u, g is set
        mul_cl = true;
    }
    memset(&fl, 0, FORMATLONGSIZ);
    if (LF_opt.l || LF_opt.s)
    {
        fl.s = (char **)lf_alloc(siz);
    }
    if (LF_opt.l || LF_opt.p)
    {
        fl.p = (char **)lf_alloc(siz);
    }
    if (LF_opt.l || LF_opt.u)
    {
        fl.u = (char **)lf_alloc(siz);
    }
    if (LF_opt.l || LF_opt.g)
    {
        fl.g = (char **)lf_alloc(siz);
    }
    if (LF_opt.l || LF_opt.i)
    {
        fl.i = (char **)lf_alloc(siz);
    }
    if (LF_opt.l || LF_opt.n)
    {
        fl.n = (char **)lf_alloc(siz);
    }
    if (mul_cl && LF_opt.m)
    { // allocate memory in case where multcol is set.
        fl.m = (char **)lf_alloc(siz);
    }

    it = lat(l, LFIRST);
    while (it)
    {
        t = (lftype)it->data;
        // user
        if (LF_opt.l || LF_opt.u)
        {
            fl.u[j] = long_user(NULL, t->st.st_uid);
            tmp = strlen(fl.u[j]);
            if (tmp > mu)
            {
                mu = tmp;
            }
        }
        // group
        if (LF_opt.l || LF_opt.g)
        {
            fl.g[j] = long_group(NULL, t->st.st_gid);
            tmp = strlen(fl.g[j]);
            if (tmp > mg)
            {
                mg = tmp;
            }
        }
        // inodes
        if (LF_opt.l || LF_opt.i)
        {
            fl.i[j] = long_ino(NULL, t->st.st_ino);
            tmp = strlen(fl.i[j]);
            if (tmp > mi)
            {
                mi = tmp;
            }
        }
        // nlink
        if (LF_opt.l || LF_opt.n)
        {
            fl.n[j] = long_nlink(NULL, t->st.st_nlink);
            tmp = strlen(fl.n[j]);
            if (tmp > mn)
            {
                mn = tmp;
            }
        }
        // size
        if (LF_opt.l || LF_opt.s)
        {
            fl.s[j] = long_size(NULL, t->st.st_size);
            tmp = strlen(fl.s[j]);
            if (tmp > ms)
            {
                ms = tmp;
            }
        }
        // permissions
        if (LF_opt.l || LF_opt.p)
        {
            fl.p[j] = long_permission(NULL, &t->st.st_mode);
            tmp = strlen(fl.p[j]);
            if (tmp > mp)
            {
                mp = tmp;
            }
        }
        if (mul_cl && LF_opt.m)
        { // modification time; max_mtime = 16 (doesn't change)
            fl.m[j] = long_mtime(NULL, &t->st.st_mtime);
        }
        linc(&it);
        ++j;
    }

    if (mul_cl)
    { // display multiple columns
        if (LF_opt.s)
        {
            cl = fl.s;
        }
        else if (LF_opt.p)
        {
            cl = fl.p;
        }
        else if (LF_opt.m)
        {
            cl = fl.m;
        }
        else if (LF_opt.i)
        {
            cl = fl.i;
        }
        else if (LF_opt.n)
        {
            cl = fl.n;
        }
        else if (LF_opt.u)
        {
            cl = fl.u;
        }
        else
        { // if (opt.g)
            cl = fl.g;
        }
        column_main(l, cl);
    }
    else
    { // display one column
        long_display(l, &fl, mp, ms, mu, mg, mi, mn);
    }
    // free memory
    if (fl.u)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.u[i]);
        }
        free(fl.u);
    }
    if (fl.g)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.g[i]);
        }
        free(fl.g);
    }
    if (fl.s)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.s[i]);
        }
        free(fl.s);
    }
    if (fl.p)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.p[i]);
        }
        free(fl.p);
    }
    if (fl.m)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.m[i]);
        }
        free(fl.m);
    }
}

char *long_ino(char *buf, ino_t data)
{
    int i = 0;
    long int n = data;

    while (n != 0)
    {
        n /= 10;
        ++i;
    }
    buf = (char *)lf_alloc(sizeof(char) * (i + 1));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_nlink(char *buf, nlink_t data)
{
    int i = 0;
    long int n = data;

    while (n != 0)
    {
        n /= 10;
        ++i;
    }
    buf = (char *)lf_alloc(sizeof(char) * (i + 1));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_size(char *buf, long int size2)
{
    double size = size2;
    int i = 0;
    long int n = size2;
    char units[10] = {'B', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', '?'};

    if (!buf)
    {
        while (n != 0)
        {
            n /= 10; // n = n/10
            ++i;
        }
        buf = (char *)lf_alloc(sizeof(char) * (i + 1));
        i = 0;
    }
    if (LF_opt.h)
    {
        while (size > 1024)
        {
            size /= 1024;
            i++;
        }
        if (size - (int)size < 0.1)
        {
            sprintf(buf, "%d%c", (int)size, units[i]);
        }
        else if (size - (int)size > 0.9)
        {
            sprintf(buf, "%d%c", (int)size + 1, units[i]);
        }
        else
        {
            sprintf(buf, "%.1f%c", size, units[i]);
        }
    }
    else
    {
        sprintf(buf, "%ld", size2);
    }

    return buf;
}

char *long_user(char *user, uid_t uid)
{
    struct passwd *pw;

    pw = getpwuid(uid);
    /**
     * 
     * to be developed, if uncknown user.
     * 
     * */
    if (!user)
    {
        user = (char *)lf_alloc(sizeof(char) * (strlen(pw->pw_name) + 1));
    }
    strcpy(user, pw->pw_name);
    return user;
}

char *long_group(char *group, gid_t gid)
{
    struct group *gr;

    gr = getgrgid(gid);
    if (!group)
    {
        group = (char *)lf_alloc(sizeof(char) * (strlen(gr->gr_name) + 1));
    }
    /**
     * 
     * to be developed, if uncknown group.
     * 
     * */
    strcpy(group, gr->gr_name);
    return group;
}

char *long_mtime(char *buf, const time_t *atm)
{
    struct tm *tm;

    if (!buf)
    {
        buf = (char *)lf_alloc(sizeof(char) * 17);
    }
    tm = localtime(atm);
    sprintf(buf, "%d-%02d-%02d %02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
    return buf;
}

char *long_permission(char *b, __mode_t *m)
{
    int i = 0;
    mode_t modes[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    if (!b)
    {
        b = (char *)lf_alloc(sizeof(char) * 12);
    }
    b[0] = filetype(m);
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
