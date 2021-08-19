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

void long_print(char *nm, int m, int f)
{ // flag  is used to adjust the data (suffix/prefix)
    int x = m - strlen(nm);

    if (f)
    {
        for (int i = 0; i < x; i++)
        {
            printf(" ");
        }
    }
    printf("%s ", nm);
    if (!f)
    {
        for (int i = 0; i < x; i++)
        {
            printf(" ");
        }
    }
}

void long_display(linklist l, format_long_t *fl,
                  int mi, int mn, int mp, int ms,
                  int mu, int mg, int ma, int mm, int mc)
{
    lf_type t;
    int j = 0;
    for (iterator i = lat(l, LFIRST); i; linc(&i))
    {
        t = (lf_type)i->data;
        // inodes
        if (LFopt.il->i)
        {
            long_print(fl->i[j], mi, false);
        }
        // nlink
        if (LFopt.il->l)
        {
            long_print(fl->l[j], mn, false);
        }
        // permissions
        if (LFopt.il->p)
        {
            long_print(fl->p[j], mp, false);
        }
        // user
        if (LFopt.il->u)
        {
            long_print(fl->u[j], mu, false);
        }
        // group
        if (LFopt.il->g)
        {
            long_print(fl->g[j], mg, false);
        }
        // size
        if (LFopt.s)
        {
            long_print(fl->s[j], ms, 1);
        }
        // access
        if (LFopt.il->a)
        {
            long_print(fl->a[j], ma, false);
        }
        // modification
        if (LFopt.il->m)
        {
            long_print(fl->m[j], mm, false);
        }
        // change
        if (LFopt.il->c)
        {
            long_print(fl->c[j], mc, false);
        }
        // file's name
        display(t->name, &t->st.st_mode, true);
        j++;
    }
}

void long_main(linklist l)
{
    format_long_t fl;
    lf_type t;
    int mi = 0, mn = 0, mu = 0, mg = 0, ms = 0, mp = 0, ma = 0, mm = 0, mc = 0;
    int j = 0, tmp;
    bool mul_cl = false;
    char **cl = NULL;
    iterator it;
    int siz = sizeof(char *) * l->count;

    if (LFopt.i &&
        (LFopt.il->i + LFopt.il->l + LFopt.il->s +
             LFopt.il->p + LFopt.il->u + LFopt.il->g +
             LFopt.il->a + LFopt.il->m + LFopt.il->c ==
         1))
    { // if only one of the options s, p, m, u, g is set
        mul_cl = true;
    }
    memset(&fl, 0, FORMATLONGSIZ);
    if (LFopt.il->i)
    {
        cl = fl.i = (char **)lf_alloc(siz);
    }
    if (LFopt.il->l)
    {
        cl = fl.l = (char **)lf_alloc(siz);
    }
    if (LFopt.il->s)
    {
        cl = fl.s = (char **)lf_alloc(siz);
    }
    if (LFopt.il->p)
    {
        cl = fl.p = (char **)lf_alloc(siz);
    }
    if (LFopt.il->u)
    {
        cl = fl.u = (char **)lf_alloc(siz);
    }
    if (LFopt.il->g)
    {
        cl = fl.g = (char **)lf_alloc(siz);
    }
    if (LFopt.il->m)
    {
        cl = fl.m = (char **)lf_alloc(siz);
    }
    if (LFopt.il->a)
    {
        cl = fl.a = (char **)lf_alloc(siz);
    }
    if (LFopt.il->c)
    {
        cl = fl.c = (char **)lf_alloc(siz);
    }

    it = lat(l, LFIRST);
    while (it)
    {
        t = (lf_type)it->data;
        // inodes
        if (LFopt.il->i)
        {
            fl.i[j] = long_ino(NULL, t->st.st_ino);
            tmp = strlen(fl.i[j]);
            if (tmp > mi)
            {
                mi = tmp;
            }
        }
        // nlink
        if (LFopt.il->l)
        {
            fl.l[j] = long_nlink(NULL, t->st.st_nlink);
            tmp = strlen(fl.l[j]);
            if (tmp > mn)
            {
                mn = tmp;
            }
        }
        // user
        if (LFopt.il->u)
        {
            fl.u[j] = long_user(NULL, t->st.st_uid);
            tmp = strlen(fl.u[j]);
            if (tmp > mu)
            {
                mu = tmp;
            }
        }
        // group
        if (LFopt.il->g)
        {
            fl.g[j] = long_group(NULL, t->st.st_gid);
            tmp = strlen(fl.g[j]);
            if (tmp > mg)
            {
                mg = tmp;
            }
        }
        // size
        if (LFopt.il->s)
        {
            fl.s[j] = long_size(NULL, t->st.st_size);
            tmp = strlen(fl.s[j]);
            if (tmp > ms)
            {
                ms = tmp;
            }
        }
        // permissions
        if (LFopt.il->p)
        {
            fl.p[j] = long_permission(NULL, &t->st.st_mode);
            tmp = strlen(fl.p[j]);
            if (tmp > mp)
            {
                mp = tmp;
            }
        }
        if (LFopt.il->m)
        { // modification time; max_mtime = 16 (doesn't change)
            fl.m[j] = long_time(NULL, &t->st.st_mtime);
            tmp = strlen(fl.m[j]);
            if (tmp > mm)
            {
                mm = tmp;
            }
        }
        if (LFopt.il->a)
        { // modification time; max_mtime = 16 (doesn't change)
            fl.a[j] = long_time(NULL, &t->st.st_atime);
            tmp = strlen(fl.a[j]);
            if (tmp > ma)
            {
                ma = tmp;
            }
        }
        if (LFopt.il->c)
        { // modification time; max_mtime = 16 (doesn't change)
            fl.c[j] = long_time(NULL, &t->st.st_ctime);
            tmp = strlen(fl.c[j]);
            if (tmp > mc)
            {
                mc = tmp;
            }
        }
        linc(&it);
        ++j;
    }

    if (mul_cl)
    { // display multiple columns
        column_main(l, cl);
    }
    else
    { // display one column
        long_display(l, &fl, mi, mn, mp, ms, mu, mg, ma, mm, mc);
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
    if (LFopt.r)
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

char *long_time(char *buf, const time_t *atm)
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
