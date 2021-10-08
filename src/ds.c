#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <errno.h>
#include "common.h"
#include "ds.h"
#include "useColumn.h"
#include "useLong.h"
#include "useTree.h"
#include "useList.h"
#include "linkedList.h"
#include "color.h"
#include "display.h"

Bool checkMode(mode_t *m)
{
    Bool ok = False;

    switch (*m & S_IFMT)
    {
    case S_IFBLK:
        if (Mparams & MB)
        {
            ok = True;
        }
        break;
    case S_IFCHR:
        if (Mparams & MC)
        {
            ok = True;
        }
        break;
    case S_IFDIR:
        if (Mparams & MD)
        {
            ok = True;
        }
        break;
    case S_IFIFO:
        if (Mparams & MP)
        {
            ok = True;
        }
        break;
    case S_IFLNK:
        if (Mparams & ML)
        {
            ok = True;
        }
        break;
    case S_IFREG:
        if (Mparams & MR)
        {
            ok = True;
        }
        break;
    case S_IFSOCK:
        if (Mparams & MS)
        {
            ok = True;
        }
        break;
    default:
        break;
    }
    if (ok && (Mparams & MU))
    { /* SUID */
        ok = S_ISUID && *m;
    }
    if (ok && (Mparams & MG))
    { /* SGID */
        ok = S_ISGID && *m;
    }
    if (ok && (Mparams & MT))
    { /* sticky bit */
        ok = S_ISVTX && *m;
    }
    if (ok && (Mparams & M1))
    { /* read by owner */
        ok = S_IRUSR & *m;
    }
    if (ok && (Mparams & M2))
    { /* write by owner */
        ok = S_IWUSR & *m;
    }
    if (ok && (Mparams & M3))
    { /* execute by owner */
        ok = S_IXUSR & *m;
    }
    if (ok && (Mparams & M4))
    { /* read by group */
        ok = S_IRGRP & *m;
    }
    if (ok && (Mparams & M5))
    { /* write by group */
        ok = S_IWGRP & *m;
    }
    if (ok && (Mparams & M6))
    { /* execute by group */
        ok = S_IXGRP & *m;
    }
    if (ok && (Mparams & M7))
    { /* read by others */
        ok = S_IROTH & *m;
    }
    if (ok && (Mparams & M8))
    { /* write by others */
        ok = S_IWOTH & *m;
    }
    if (ok && (Mparams & M9))
    { /* execute by others */
        ok = S_IXOTH & *m;
    }
    return ok;
}
void core(TreeInfo *tree)
{
    DIR *dir = opendir(Pth);
    struct dirent *item;
    struct stat s;
    linkedList filesList = lOpen();
    File file;
    long int cnt = 0;
    Bool ok, errOccured;

    // keep value of "path" and "path_z"
    if (!dir)
    {
        strcpy(Bfr, strerror(errno));
        if (Opts & OT)
        {
            printBranch(tree, True);
            printf("access denied: %s\n", Bfr);
        }
        else
        {
            printf("%s: access denied to '%s': %s\n", PROGRAM, Pth, Bfr);
        }
        return;
    }

    while ((item = readdir(dir)))
    {
        ok = False;
        errOccured = True;
        if ((Mparams & MH) || (item->d_name[0] != '.'))
        {
            strcpy(&Pth[PthLen], item->d_name);
            if (fileStat(Pth, &s))
            {
                if (checkMode(&s.st_mode))
                {
                    ok = True;
                    errOccured = False;
                }
            }
            if (ok)
            {
                if (Opts & OC)
                {
                    ++cnt;
                }
                else
                {
                    file = (File)memAlloc(FILE_SIZE);
                    file->name = (char *)memAlloc((strlen(item->d_name) + 1));
                    file->st = s;
                    file->err = errOccured;
                    strcpy(file->name, item->d_name);
                    lInsert(filesList, LLAST, file);
                }
            }
        }
    }
    closedir(dir);
    if (Opts & OC)
    {
        printf("%ld\n", cnt);
        return;
    }
    if (lEmpty(filesList))
    {
        return;
    }
    if (!(Sparams & SD))
    {
        Sort(filesList);
    }
    if (Opts & OT)
    { // format tree
        treeMain(filesList, tree);
    }
    else if (Opts & OL)
    { // format long
        longMain(filesList);
    }
    else if (Opts & O1 || Opts & O2 || Opts & O3 || Opts & O4)
    { // format long
        listMain(filesList, NULL, 0);
    }
    else
    { // format column
        columnMain(filesList, NULL, 0);
    }
    for (Iterator it = lAt(filesList, LFIRST); it; lInc(&it))
    {
        file = (File)it->data;
        free(file->name);
        free(file);
    }
    lClose(filesList);
}

int sortNames(File f1, File f2)
{
    return strCompare(f1->name, f2->name);
}
int sortI(File f1, File f2)
{
    return (f1->st.st_ino < f2->st.st_ino) ? -1 : 1;
}
int sortN(File f1, File f2)
{
    return (f1->st.st_nlink < f2->st.st_nlink) ? -1 : 1;
}
int sortU(File f1, File f2)
{
    return (f1->st.st_uid < f2->st.st_uid) ? -1 : 1;
}
int sortG(File f1, File f2)
{
    return (f1->st.st_gid < f2->st.st_gid) ? -1 : 1;
}
int sortS(File f1, File f2)
{
    return (f1->st.st_size < f2->st.st_size) ? -1 : 1;
}
int sortA(File f1, File f2)
{
    return (f1->st.st_atime < f2->st.st_atime) ? -1 : 1;
}
int sortM(File f1, File f2)
{
    return (f1->st.st_mtime < f2->st.st_mtime) ? -1 : 1;
}
int sortC(File f1, File f2)
{
    return (f1->st.st_ctime < f2->st.st_ctime) ? -1 : 1;
}
int sortT(File f1, File f2)
{
    return ((f1->st.st_mode) < (f2->st.st_mode)) ? -1 : 1;
}
int sortE(File f1, File f2)
{
    char *s1 = fileExtension(f1->name), *s2 = fileExtension(f2->name);
    if (s1 && s2 && strcmp(s1, s2))
    {
        return strCompare(s1, s2);
    }
    return strCompare(f1->name, f2->name);
}

void Sort(linkedList l)
{
    lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortNames);

    switch (Sparams)
    {
    case SI:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortI);
        break;
    case SN:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortN);
        break;
    case SU:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortU);
        break;
    case SG:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortG);
        break;
    case SS:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortS);
        break;
    case SM:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortM);
        break;
    case SA:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortA);
        break;
    case SC:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortC);
        break;
    case ST:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortT);
        break;
    case SE:
        lSort(l, LFIRST, LLAST, (int (*)(void *, void *))sortE);
        break;
    default:
        break;
    }
}
