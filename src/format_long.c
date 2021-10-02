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

void long_display(linklist l, _file_info *files_info, int index, int mi, int mn, int mu, int mg, int mp, int ms, int ma, int mm, int mc)
{
    _file file;
    int i = 0;

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;

        // inodes
        if (_opt.ll->i)
        {
            long_print(files_info[i].bfr[index], mi, _false);
        }
        // nlink
        if (_opt.ll->n)
        {
            long_print(files_info[i].bfr[index], mn, _false);
        }
        // permissions
        if (_opt.ll->p)
        {
            long_print(files_info[i].bfr[index], mp, _false);
        }
        // size
        if (_opt.ll->s || _opt.ll->r)
        {
            long_print(files_info[i].bfr[index], ms, _true);
        }
        // user
        if (_opt.ll->u)
        {
            long_print(files_info[i].bfr[index], mu, _false);
        }
        // group
        if (_opt.ll->g)
        {
            long_print(files_info[i].bfr[index], mg, _false);
        }
        // access
        if (_opt.ll->a)
        {
            long_print(files_info[i].bfr[index], ma, _false);
        }
        // modification
        if (_opt.ll->m)
        {
            long_print(files_info[i].bfr[index], mm, _false);
        }
        // change
        if (_opt.ll->c)
        {
            long_print(files_info[i].bfr[index], mc, _false);
        }
        // file's name
        display(file->name, &file->st.st_mode, _true);
    }
}

void long_main(linklist l)
{
    _file_info *files_info;
    _file file;
    int i = 0, tmp;
    int mi = 1, mn = 1, mu = 1, mg = 1, ms = 1, mp = 1, ma = 1, mm = 1, mc = 1;

    int col_index;
    int col_cnt = 0;
    if (_opt.ll->i)
    {
        ++col_cnt;
        col_index = I_INDEX;
    }
    if (_opt.ll->n)
    {
        ++col_cnt;
        col_index = N_INDEX;
    }
    if (_opt.ll->u)
    {
        ++col_cnt;
        col_index = U_INDEX;
    }
    if (_opt.ll->g)
    {
        ++col_cnt;
        col_index = G_INDEX;
    }
    if (_opt.ll->r || _opt.ll->s)
    {
        ++col_cnt;
        col_index = S_INDEX;
    }
    if (_opt.ll->p)
    {
        ++col_cnt;
        col_index = P_INDEX;
    }
    if (_opt.ll->a)
    {
        ++col_cnt;
        col_index = A_INDEX;
    }
    if (_opt.ll->m)
    {
        ++col_cnt;
        col_index = M_INDEX;
    }
    if (_opt.ll->c)
    {
        ++col_cnt;
        col_index = C_INDEX;
    }
    // set default maximums sizes.
    files_info = (_file_info *)_alloc(_FILE_INFO_SIZE * l->count);
    memset(&files_info, 0, _FILE_INFO_SIZE * l->count);

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (_file)it->data;
        // inodes
        if (_opt.ll->i)
        {
            files_info[i].bfr[I_INDEX] = long_ino(NULL, file->st.st_ino);
            tmp = strlen(files_info[i].bfr[I_INDEX]);
            if (tmp > mi)
            {
                mi = tmp;
            }
        }
        // nlink
        if (_opt.ll->n)
        {
            files_info[i].bfr[N_INDEX] = long_nlink(NULL, file->st.st_nlink);
            tmp = strlen(files_info[i].bfr[N_INDEX]);
            if (tmp > mn)
            {
                mn = tmp;
            }
        }
        // user
        if (_opt.ll->u)
        {
            files_info[i].bfr[U_INDEX] = long_user(NULL, file->st.st_uid);
            tmp = strlen(files_info[i].bfr[U_INDEX]);
            if (tmp > mu)
            {
                mu = tmp;
            }
        }
        // group
        if (_opt.ll->g)
        {
            files_info[i].bfr[G_INDEX] = long_group(NULL, file->st.st_gid);
            tmp = strlen(files_info[i].bfr[G_INDEX]);
            if (tmp > mg)
            {
                mg = tmp;
            }
        }
        // size and readable size
        if (_opt.ll->r || _opt.ll->s)
        {
            if (_opt.ll->r)
            {
                files_info[i].bfr[S_INDEX] = long_size_readable(NULL, file->st.st_size);
            }
            else
            {
                files_info[i].bfr[S_INDEX] = long_size(NULL, file->st.st_size);
            }
            tmp = strlen(files_info[i].bfr[S_INDEX]);
            if (tmp > ms)
            {
                ms = tmp;
            }
        }
        // permissions
        if (_opt.ll->p)
        {
            files_info[i].bfr[P_INDEX] = long_permission(NULL, &file->st.st_mode);
            tmp = strlen(files_info[i].bfr[P_INDEX]);
            if (tmp > mp)
            {
                mp = tmp;
            }
        }
        if (_opt.ll->m)
        { // modification time; max_mtime = 16

            files_info[i].bfr[M_INDEX] = long_time(NULL, &file->st.st_mtime);
            tmp = strlen(files_info[i].bfr[M_INDEX]);
            if (tmp > mm)
            {
                mm = tmp;
            }
        }
        if (_opt.ll->a)
        { // modification time; max_mtime = 16

            files_info[i].bfr[A_INDEX] = long_time(NULL, &file->st.st_atime);
            tmp = strlen(files_info[i].bfr[A_INDEX]);
            if (tmp > ma)
            {
                ma = tmp;
            }
        }
        if (_opt.ll->c)
        { // modification time; max_mtime = 16

            files_info[i].bfr[C_INDEX] = long_time(NULL, &file->st.st_ctime);
            tmp = strlen(files_info[i].bfr[C_INDEX]);
            if (tmp > mc)
            {
                mc = tmp;
            }
        }
    }

    if (col_cnt)
    { // display multiple columns
        if (_opt._2 || _opt._1 || _opt._3 || _opt._4)
        {
            list_main(l, files_info, col_index);
        }
        else
        {
            column_main(l, files_info, col_index);
        }
    }
    else
    { // display one column
        long_display(l, files_info, col_index, mi, mn, mu, mg, mp, ms, ma, mm, mc);
    }
    free(files_info);
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
