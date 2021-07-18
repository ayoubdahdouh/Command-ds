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

void long_selector(char *fname, int max)
{
    int tmp = max - strlen(fname);
    if (opt.s)
    {
        for (int i = 0; i < tmp; i++)
        {
            printf(" ");
        }
    }
    printf("%s ", fname);
    if (!opt.s)
    {
        for (int i = 0; i < tmp; i++)
        {
            printf(" ");
        }
    }
}
void long_display(char **tb, format_long_t *fl, int tbsiz, int max_user, int max_group, int max_size, int max_perm)
{
    char buf[20];
    for (int i = 0; i < tbsiz; i++)
    {
        // type of file.
        if (opt.l)
        {
            switch (fl->st[i].st_mode & S_IFMT)
            {
            case S_IFBLK:
                printf("b ");
                break;
            case S_IFCHR:
                printf("c ");
                break;
            case S_IFDIR:
                printf("d ");
                break;
            case S_IFIFO:
                printf("p ");
                break;
            case S_IFLNK:
                printf("l ");
                break;
            case S_IFREG:
                printf("- ");
                break;
            case S_IFSOCK:
                printf("s ");
                break;
            default:
                printf("? ");
                break;
            }
        }
        if (opt.l || opt.p)
        {
            long_selector(fl->perm[i], max_perm);
        }
        if (opt.l || opt.u)
        {
            long_selector(fl->user[i], max_user);
        }
        if (opt.l || opt.g)
        {
            long_selector(fl->group[i], max_group);
        }
        if (opt.l || opt.m)
        {
            long_mtime(buf, &(fl->st[i].st_mtime));
            printf("%s ", buf);
        }
        if (opt.l || opt.s)
        {
            long_selector(fl->size[i], max_size);
        }
        lf_print(tb[i], &fl->st[i].st_mode);
    }
}

void long_main(char **tb, int tb_f, int tb_d, int tbsiz)
{
    struct stat s;
    format_long_t fl;
    int max_user = 0, max_group = 0, max_size = 0, max_perm = 0;
    int tmp;
    int multcol = 0;
    char **secondcol = NULL;

    if (!opt.l && (opt.s ^ opt.p ^ opt.m ^ opt.u ^ opt.g))
    { // if only one of the options s,p,u,g,m  is setted
        multcol = 1;
    }

    fl.st = (struct stat *)lf_malloc(sizeof(struct stat) * tbsiz);
    if (opt.l || opt.s)
    {
        fl.size = (char **)lf_malloc(sizeof(char *) * tbsiz);
    }
    if (opt.l || opt.p)
    {
        fl.perm = (char **)lf_malloc(sizeof(char *) * tbsiz);
    }
    if (multcol && opt.m)
    { // allocate memory in case where multcol is set.
        fl.mtime = (char **)lf_malloc(sizeof(char *) * tbsiz);
    }
    if (opt.l || opt.u)
    {
        fl.user = (char **)lf_malloc(sizeof(char *) * tbsiz);
    }
    if (opt.l || opt.g)
    {
        fl.group = (char **)lf_malloc(sizeof(char *) * tbsiz);
    }

    for (int i = 0; i < tbsiz; i++)
    {
        strcpy(&path[pathsiz], tb[i]);

        lf_stat(path, &s);
        fl.st[i] = s;
        // user
        if (opt.l || opt.u)
        {
            fl.user[i] = long_user(NULL, s.st_uid);
            tmp = strlen(fl.user[i]);
            if (tmp > max_user)
            {
                max_user = tmp;
            }
        }
        // group
        if (opt.l || opt.g)
        {
            fl.group[i] = long_group(NULL, s.st_gid);
            tmp = strlen(fl.group[i]);
            if (tmp > max_group)
            {
                max_group = tmp;
            }
        }
        // size
        if (opt.l || opt.s)
        {
            fl.size[i] = long_size(NULL, s.st_size);
            tmp = strlen(fl.size[i]);
            if (tmp > max_size)
            {
                max_size = tmp;
            }
        }
        // permissions
        if (opt.l || opt.p)
        {
            fl.perm[i] = long_permission(NULL, &s.st_mode);
            tmp = strlen(fl.perm[i]);
            if (tmp > max_perm)
            {
                max_perm = tmp;
            }
        }
        if (multcol && opt.m)
        { // modification time.
            fl.mtime[i] = long_mtime(NULL, &(s.st_mtime));
            // max_mtime = 16 (doesn't change).
        }
    }
    if (multcol && opt.s)
    {
        secondcol = fl.size;
    }
    if (multcol && opt.p)
    {
        secondcol = fl.perm;
    }
    if (multcol && opt.m)
    {
        secondcol = fl.mtime;
    }
    if (multcol && opt.u)
    {
        secondcol = fl.user;
    }
    if (multcol && opt.g)
    {
        secondcol = fl.group;
    }
    if (multcol)
    { // display multiple columns
        column_main(tb, secondcol, tbsiz);
    }
    else
    { // display one column
        long_display(tb, &fl, tbsiz, max_user, max_group, max_size, max_perm);
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
        buf = (char *)lf_malloc(sizeof(char) * (i + 1));
        i = 0;
    }
    if (opt.h)
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
        user = (char *)lf_malloc(sizeof(char) * (strlen(pw->pw_name) + 1));
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
        group = (char *)lf_malloc(sizeof(char) * (strlen(gr->gr_name) + 1));
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
    struct tm tm;

    if (!buf)
    {
        buf = (char *)lf_malloc(sizeof(char) * 17);
    }
    tm = *localtime(atm);
    sprintf(buf, "%d-%02d-%02d %02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    return buf;
}

char *long_permission(char *buf, __mode_t *mode)
{
    int i = 0;
    __mode_t modes[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    if (!buf)
    {
        buf = (char *)lf_malloc(sizeof(char) * 11);
    }

    for (i = 0; i < 9; i += 3)
    {
        if (*mode & modes[i])
        {
            buf[i] = 'r';
        }
        else
        {
            buf[i] = '-';
        }
        if (*mode & modes[i + 1])
        {
            buf[i + 1] = 'w';
        }
        else
        {
            buf[i + 1] = '-';
        }
        if (*mode & modes[i + 2])
        {
            buf[i + 2] = 'x';
        }
        else
        {
            buf[i + 2] = '-';
        }
    }
    if (*mode & S_ISUID)
    {
        buf[2] = 's';
    }
    if (*mode & S_ISGID)
    {
        buf[5] = 's';
    }
    if (*mode & S_ISVTX)
    {
        buf[9] = '-';
        buf[10] = 0;
    }
    else
    {
        buf[9] = 0;
    }

    return buf;
}
