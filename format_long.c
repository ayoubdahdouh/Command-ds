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

void long_display(linklist l, format_long_t *fl, int max_user, int max_group, int max_size, int max_perm)
{
    char buf[20];
    lftype t;
    int j = 0;
    for (iterator i = lat(l, LFIRST); i; linc(&i))
    {
        t = (lftype)i->data;
        if (LF_opt.l || LF_opt.p)
        {
            long_print(fl->perm[j], max_perm, 0);
        }
        if (LF_opt.l || LF_opt.u)
        {
            long_print(fl->user[j], max_user, 0);
        }
        if (LF_opt.l || LF_opt.g)
        {
            long_print(fl->group[j], max_group, 0);
        }
        if (LF_opt.l || LF_opt.m)
        {
            long_mtime(buf, &t->st.st_mtime);
            printf("%s ", buf);
        }
        if (LF_opt.l || LF_opt.s)
        {
            long_print(fl->size[j], max_size, 1);
        }
        lfprint(t->name, &t->st.st_mode, true, true);
        j++;
    }
}

void long_main(linklist l)
{
    format_long_t fl;
    lftype t;
    int max_user = 0, max_group = 0, max_size = 0, max_perm = 0, j = 0, tmp, multcol = 0;
    char **secondcol = NULL;
    iterator it;

    if (!LF_opt.l && (LF_opt.s ^ LF_opt.p ^ LF_opt.m ^ LF_opt.u ^ LF_opt.g))
    { // if only one of the options s, p, m, u, g is set
        multcol = 1;
    }
    memset(&fl, 0, FORMATLONGSIZ);
    if (LF_opt.l || LF_opt.s)
    {
        fl.size = (char **)lf_alloc(sizeof(char *) * l->count);
    }
    if (LF_opt.l || LF_opt.p)
    {
        fl.perm = (char **)lf_alloc(sizeof(char *) * l->count);
    }
    if (LF_opt.l || LF_opt.u)
    {
        fl.user = (char **)lf_alloc(sizeof(char *) * l->count);
    }
    if (LF_opt.l || LF_opt.g)
    {
        fl.group = (char **)lf_alloc(sizeof(char *) * l->count);
    }
    if (multcol && LF_opt.m)
    { // allocate memory in case where multcol is set.
        fl.mtime = (char **)lf_alloc(sizeof(char *) * l->count);
    }

    it = lat(l, LFIRST);
    while (it)
    {
        t = (lftype)it->data;
        // user
        if (LF_opt.l || LF_opt.u)
        {
            fl.user[j] = long_user(NULL, t->st.st_uid);
            tmp = strlen(fl.user[j]);
            if (tmp > max_user)
            {
                max_user = tmp;
            }
        }
        // group
        if (LF_opt.l || LF_opt.g)
        {
            fl.group[j] = long_group(NULL, t->st.st_gid);
            tmp = strlen(fl.group[j]);
            if (tmp > max_group)
            {
                max_group = tmp;
            }
        }
        // size
        if (LF_opt.l || LF_opt.s)
        {
            fl.size[j] = long_size(NULL, t->st.st_size);
            tmp = strlen(fl.size[j]);
            if (tmp > max_size)
            {
                max_size = tmp;
            }
        }
        // permissions
        if (LF_opt.l || LF_opt.p)
        {
            fl.perm[j] = long_permission(NULL, &t->st.st_mode);
            tmp = strlen(fl.perm[j]);
            if (tmp > max_perm)
            {
                max_perm = tmp;
            }
        }
        if (multcol && LF_opt.m)
        { // modification time; max_mtime = 16 (doesn't change)
            fl.mtime[j] = long_mtime(NULL, &t->st.st_mtime);
        }
        linc(&it);
        ++j;
    }

    if (multcol)
    { // display multiple columns
        if (LF_opt.s)
        {
            secondcol = fl.size;
        }
        else if (LF_opt.p)
        {
            secondcol = fl.perm;
        }
        else if (LF_opt.m)
        {
            secondcol = fl.mtime;
        }
        else if (LF_opt.u)
        {
            secondcol = fl.user;
        }
        else
        { // if (opt.g)
            secondcol = fl.group;
        }
        column_main(l, secondcol);
    }
    else
    { // display one column
        long_display(l, &fl, max_user, max_group, max_size, max_perm);
    }
    // free memory
    if (fl.user)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.user[i]);
        }
        free(fl.user);
    }
    if (fl.group)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.group[i]);
        }
        free(fl.group);
    }
    if (fl.size)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.size[i]);
        }
        free(fl.size);
    }
    if (fl.perm)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.perm[i]);
        }
        free(fl.perm);
    }
    if (fl.mtime)
    {
        for (int i = 0; i < l->count; i++)
        {
            free(fl.mtime[i]);
        }
        free(fl.mtime);
    }
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
