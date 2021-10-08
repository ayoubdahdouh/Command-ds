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

void long_print(char *nm, int m, Bool rtl)
{
    if (m <= 0)
    {
        return;
    }

    // rtl: left to right
    int x = m - strlen(nm);

    if (rtl)
    {
        while (x > 0)
        {
            printf(" ");
            --x;
        }
    }
    if (!nm)
    {
        printf("? ");
    }
    else
    {
        printf("%s ", nm);
    }

    if (!rtl)
    {
        while (x > 0)
        {
            printf(" ");
            --x;
        }
    }
}

void long_display(linklist l, FileInfo *files_info, int mi, int mn, int mu, int mg, int mp, int ms, int ma, int mm, int mc)
{
    File file;
    int i = 0;

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (File)it->data;

        // inodes
        if (Lparams & LI)
        {
            long_print(files_info[i].bfr[I_INDEX], mi, False);
        }
        // nlink
        if (Lparams & LN)
        {
            long_print(files_info[i].bfr[N_INDEX], mn, False);
        }
        // user
        if (Lparams & LU)
        {
            long_print(files_info[i].bfr[U_INDEX], mu, False);
        }
        // group
        if (Lparams & LG)
        {
            long_print(files_info[i].bfr[G_INDEX], mg, False);
        }
        // permissions
        if (Lparams & LP)
        {
            long_print(files_info[i].bfr[P_INDEX], mp, False);
        }
        // size
        if (Lparams & LS || Lparams & LR)
        {
            long_print(files_info[i].bfr[S_INDEX], ms, True);
        }
        // access
        if (Lparams & LA)
        {
            long_print(files_info[i].bfr[A_INDEX], ma, False);
        }
        // modification
        if (Lparams & LM)
        {
            long_print(files_info[i].bfr[M_INDEX], mm, False);
        }
        // change
        if (Lparams & LC)
        {
            long_print(files_info[i].bfr[C_INDEX], mc, False);
        }
        // file's name
        display(file->name, &file->st.st_mode, True);
    }
}

int max_length(FileInfo *files_info, int n, int index)
{
    int max = 0, len;
    // verify that fieds not empties.
    // it's enough to verify the first one.
    if (files_info[0].bfr[index])
    {
        for (int i = 0; i < n; i++)
        {
            len = strlen(files_info[i].bfr[index]);
            if (max < len)
            {
                max = len;
            }
        }
    }
    return max;
}
void long_main(linklist l)
{
    FileInfo *files_info;
    File file;
    int i = 0;
    int mi = 1, mn = 1, mu = 1, mg = 1, ms = 1, mp = 1, ma = 1, mm = 1, mc = 1;

    int col_index;
    int col_cnt = 0;
    if (Lparams & LI)
    {
        ++col_cnt;
        col_index = I_INDEX;
    }
    if (Lparams & LN)
    {
        ++col_cnt;
        col_index = N_INDEX;
    }
    if (Lparams & LU)
    {
        ++col_cnt;
        col_index = U_INDEX;
    }
    if (Lparams & LG)
    {
        ++col_cnt;
        col_index = G_INDEX;
    }
    if ((Lparams & LR) || (Lparams & LS))
    {
        ++col_cnt;
        col_index = S_INDEX;
    }
    if (Lparams & LP)
    {
        ++col_cnt;
        col_index = P_INDEX;
    }
    if (Lparams & LA)
    {
        ++col_cnt;
        col_index = A_INDEX;
    }
    if (Lparams & LM)
    {
        ++col_cnt;
        col_index = M_INDEX;
    }
    if (Lparams & LC)
    {
        ++col_cnt;
        col_index = C_INDEX;
    }
    // set default maximums sizes.
    files_info = (FileInfo *)Alloc(_FILE_INFO_SIZE * l->count);
    memset(files_info, 0, _FILE_INFO_SIZE * l->count);

    for (iterator it = lat(l, LFIRST); it; linc(&it), ++i)
    {
        file = (File)it->data;
        if (Lparams & LI)
        { // inodes
            files_info[i].bfr[I_INDEX] = long_ino(NULL, file->st.st_ino);
        }
        if (Lparams & LN)
        { // nlink
            files_info[i].bfr[N_INDEX] = long_nlink(NULL, file->st.st_nlink);
        }
        if (Lparams & LU)
        { // user
            files_info[i].bfr[U_INDEX] = long_user(NULL, file->st.st_uid);
        }
        if (Lparams & LG)
        { // group
            files_info[i].bfr[G_INDEX] = long_group(NULL, file->st.st_gid);
        }
        if (Lparams & LS)
        { // dable size
            files_info[i].bfr[S_INDEX] = long_size(NULL, file->st.st_size);
        }
        else if (Lparams & LR)
        { // size
            files_info[i].bfr[S_INDEX] = long_size_readable(NULL, file->st.st_size);
        }
        if (Lparams & LP)
        { // permissions
            files_info[i].bfr[P_INDEX] = long_permission(NULL, &file->st.st_mode);
        }
        if (Lparams & LM)
        { // modification time

            files_info[i].bfr[M_INDEX] = long_time(NULL, &file->st.st_mtime);
        }
        if (Lparams & LA)
        { // access time

            files_info[i].bfr[A_INDEX] = long_time(NULL, &file->st.st_atime);
        }
        if (Lparams & LC)
        { // change status time
            files_info[i].bfr[C_INDEX] = long_time(NULL, &file->st.st_ctime);
        }
    }
    if (col_cnt == 1)
    { // display multiple columns
        if (Opts & O2 || Opts & O1 || Opts & O3 || Opts & O4)
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
        mi = max_length(files_info, l->count, I_INDEX);
        mn = max_length(files_info, l->count, N_INDEX);
        mu = max_length(files_info, l->count, U_INDEX);
        mg = max_length(files_info, l->count, G_INDEX);
        mp = max_length(files_info, l->count, P_INDEX);
        ms = max_length(files_info, l->count, S_INDEX);
        ma = max_length(files_info, l->count, A_INDEX);
        mm = max_length(files_info, l->count, M_INDEX);
        mc = max_length(files_info, l->count, C_INDEX);
        long_display(l, files_info, mi ? mi : 1, mn ? mn : 1, mu ? mu : 1, mg ? mg : 1, mp ? mp : 1, ms ? ms : 1, ma ? ma : 1, mm ? mm : 1, mc ? mc : 1);
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
    buf = (char *)Alloc(number_size(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *long_nlink(char *buf, nlink_t data)
{
    buf = (char *)Alloc(number_size(data));
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
        buf = (char *)Alloc(number_size(data));
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
        buf = (char *)Alloc(number_size(data));
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
            user = (char *)Alloc(2);
        }
        user[0] = '?';
        user[1] = 0;
        return user;
    }
    if (!user)
    {
        user = (char *)Alloc((strlen(pw->pw_name) + 1));
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
            group = (char *)Alloc(2);
        }
        group[0] = '?';
        group[1] = 0;
        return group;
    }
    if (!group)
    {
        group = (char *)Alloc((strlen(grp->gr_name) + 1));
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
    while (!(result = strftime(tmp, buflen, TimeStyle, t)) && (tmp[0] == 0) && repeat)
    {
        --repeat;
        buflen += 100;
        tmp = realloc(tmp, buflen);
    }
    if (repeat == 0 && result == 0)
    {
        Quit("%s: error: can't allocate enough memory to handle the time style");
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
        b = (char *)Alloc(12);
    }
    b[0] = fileType(m);
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
