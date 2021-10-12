#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "useColumn.h"
#include "useLong.h"
#include "useList.h"
#include "display.h"
#include "common.h"

void printFormattedValue(char *nm, int m, Bool rtl)
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

void longDisplay(linkedList l, FileInfo *filesInfo, int max[])
{
    File file;
    int i = 0;
    int tmp;
    // show columns header if neccessaryß
    if (Opts & OI)
    {
        char *bfr[9] = {"Inode", "Links", "User", "Group", "Permissions",
                        "Size", "Access", "Modification", "Change"};
        for (int i = 0; i < 9; ++i)
        {
            if (Lparams & (1 << i))
            {
                if (i != 0 && i != 1 && i != 5)
                {
                    printf("\e[4m%s\e[0m ", bfr[i]);
                }
                tmp = strlen(bfr[i]);
                if (max[i] <= tmp)
                {
                    max[i] = tmp;
                }
                else
                {
                    for (int j = 0; j < max[i] - tmp; ++j)
                    {
                        printf(" ");
                    }
                }
                if (i == 0 || i == 1 || i == 5)
                {
                    printf("\e[4m%s\e[0m ", bfr[i]);
                }
            }
        }
        printf("\e[4mName\e[0m\n");
    }

    for (Iterator it = lAt(l, LFIRST); it; lInc(&it), ++i)
    {
        file = (File)it->data;

        // inodes
        if (Lparams & LI)
        {
            printFormattedValue(filesInfo[i].bfr[I_INDEX], max[0], True);
        }
        // nlink
        if (Lparams & LN)
        {
            printFormattedValue(filesInfo[i].bfr[N_INDEX], max[1], True);
        }
        // user
        if (Lparams & LU)
        {
            printFormattedValue(filesInfo[i].bfr[U_INDEX], max[2], False);
        }
        // group
        if (Lparams & LG)
        {
            printFormattedValue(filesInfo[i].bfr[G_INDEX], max[3], False);
        }
        // permissions
        if (Lparams & LP)
        {
            printFormattedValue(filesInfo[i].bfr[P_INDEX], max[4], False);
        }
        // size
        if (Lparams & LS)
        {
            printFormattedValue(filesInfo[i].bfr[S_INDEX], max[5], True);
        }
        // access
        if (Lparams & LA)
        {
            printFormattedValue(filesInfo[i].bfr[A_INDEX], max[6], False);
        }
        // modification
        if (Lparams & LM)
        {
            printFormattedValue(filesInfo[i].bfr[M_INDEX], max[7], False);
        }
        // change
        if (Lparams & LC)
        {
            printFormattedValue(filesInfo[i].bfr[C_INDEX], max[8], False);
        }
        // file's name
        display(file->name, &file->st.st_mode, True);
    }
}

int maxLength(FileInfo *filesInfo, int n, int index)
{
    int max = 0, len;
    // verify that fieds not empties.
    // it's enough to verify the first one.
    if (filesInfo[0].bfr[index])
    {
        for (int i = 0; i < n; i++)
        {
            len = strlen(filesInfo[i].bfr[index]);
            if (max < len)
            {
                max = len;
            }
        }
    }
    return max;
}
void longMain(linkedList l)
{
    FileInfo *filesInfo;
    File file;
    int i = 0;

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
    if (Lparams & LS)
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
    filesInfo = (FileInfo *)memAlloc(FILEINFO_SIZE * l->count);
    memset(filesInfo, 0, FILEINFO_SIZE * l->count);

    for (Iterator it = lAt(l, LFIRST); it; lInc(&it), ++i)
    {
        file = (File)it->data;
        if (Lparams & LI)
        { // inodes
            filesInfo[i].bfr[I_INDEX] = getIno(NULL, file->st.st_ino);
        }
        if (Lparams & LN)
        { // nlink
            filesInfo[i].bfr[N_INDEX] = getNlink(NULL, file->st.st_nlink);
        }
        if (Lparams & LU)
        { // user
            filesInfo[i].bfr[U_INDEX] = getUser(NULL, file->st.st_uid);
        }
        if (Lparams & LG)
        { // group
            filesInfo[i].bfr[G_INDEX] = getGroup(NULL, file->st.st_gid);
        }
        if (Lparams & LS)
        {
            if (Lparams & LR)
            { // humain readable size
                filesInfo[i].bfr[S_INDEX] = getReadableSize(NULL, file->st.st_size);
            }
            else
            {
                filesInfo[i].bfr[S_INDEX] = getSize(NULL, file->st.st_size);
            }
        }
        if (Lparams & LP)
        { // permissions
            filesInfo[i].bfr[P_INDEX] = getPermissions(NULL, &file->st.st_mode);
        }
        if (Lparams & LM)
        { // modification time

            filesInfo[i].bfr[M_INDEX] = getTime(NULL, &file->st.st_mtime);
        }
        if (Lparams & LA)
        { // access time

            filesInfo[i].bfr[A_INDEX] = getTime(NULL, &file->st.st_atime);
        }
        if (Lparams & LC)
        { // change status time
            filesInfo[i].bfr[C_INDEX] = getTime(NULL, &file->st.st_ctime);
        }
    }
    if (col_cnt == 1)
    { // display multiple columns
        if (Opts & O2 || Opts & O1 || Opts & O3 || Opts & O4)
        {
            listMain(l, filesInfo, col_index);
        }
        else
        {
            columnMain(l, filesInfo, col_index);
        }
    }
    else
    { // display one column
        int max[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        max[0] = maxLength(filesInfo, l->count, I_INDEX);
        max[1] = maxLength(filesInfo, l->count, N_INDEX);
        max[2] = maxLength(filesInfo, l->count, U_INDEX);
        max[3] = maxLength(filesInfo, l->count, G_INDEX);
        max[4] = maxLength(filesInfo, l->count, P_INDEX);
        max[5] = maxLength(filesInfo, l->count, S_INDEX);
        max[6] = maxLength(filesInfo, l->count, A_INDEX);
        max[7] = maxLength(filesInfo, l->count, M_INDEX);
        max[8] = maxLength(filesInfo, l->count, C_INDEX);
        longDisplay(l, filesInfo, max);
    }
    free(filesInfo);
}

int nbrOfDigits(long int n)
{
    int i = 1;
    while (n != 0)
    {
        n /= 10;
        ++i;
    }
    return i;
}

char *getIno(char *buf, ino_t data)
{
    buf = (char *)memAlloc(nbrOfDigits(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *getNlink(char *buf, nlink_t data)
{
    buf = (char *)memAlloc(nbrOfDigits(data));
    sprintf(buf, "%ld", data);
    return buf;
}

char *getReadableSize(char *buf, long int data)
{
    double cnt = data;
    int i = 0;
    char units[10] = {'B', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', '?'};

    if (!buf)
    {
        buf = (char *)memAlloc(nbrOfDigits(data));
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
char *getSize(char *buf, long int data)
{
    if (!buf)
    {
        buf = (char *)memAlloc(nbrOfDigits(data));
    }
    sprintf(buf, "%ld", data);

    return buf;
}

char *getUser(char *user, uid_t uid)
{
    struct passwd *pw;

    pw = getpwuid(uid);
    if (!pw)
    {
        if (!user)
        {
            user = (char *)memAlloc(2);
        }
        user[0] = '?';
        user[1] = 0;
        return user;
    }
    if (!user)
    {
        user = (char *)memAlloc((strlen(pw->pw_name) + 1));
    }
    strcpy(user, pw->pw_name);
    return user;
}

char *getGroup(char *group, gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    if (!grp)
    {
        if (!group)
        {
            group = (char *)memAlloc(2);
        }
        group[0] = '?';
        group[1] = 0;
        return group;
    }
    if (!group)
    {
        group = (char *)memAlloc((strlen(grp->gr_name) + 1));
    }
    strcpy(group, grp->gr_name);

    return group;
}

char *getTime(char *buf, const time_t *atm)
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
        quitProgram("%s: error: can't allocate enough memory to handle the time style");
    }

    if (!buf)
    {
        buf = (char *)realloc(tmp, strlen(tmp) + 1);
    }
    return buf;
}

char *getPermissions(char *b, __mode_t *m)
{
    int i = 0;
    mode_t modes[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    if (!b)
    {
        b = (char *)memAlloc(12);
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
