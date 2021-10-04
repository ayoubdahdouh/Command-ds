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
#include "lf.h"
#include "format_column.h"
#include "format_long.h"
#include "format_tree.h"
#include "format_list.h"
#include "list.h"
#include "color.h"
#include "display.h"

_bool validate_mode(mode_t *mode)
{
    _bool ok = _false;

    switch (*mode & S_IFMT)
    {
    case S_IFBLK:
        if (_opt.ml->b)
        {
            ok = _true;
        }
        break;
    case S_IFCHR:
        if (_opt.ml->c)
        {
            ok = _true;
        }
        break;
    case S_IFDIR:
        if (_opt.ml->d)
        {
            ok = _true;
        }
        break;
    case S_IFIFO:
        if (_opt.ml->p)
        {
            ok = _true;
        }
        break;
    case S_IFLNK:
        if (_opt.ml->l)
        {
            ok = _true;
        }
        break;
    case S_IFREG:
        if (_opt.ml->r)
        {
            ok = _true;
        }
        break;
    case S_IFSOCK:
        if (_opt.ml->s)
        {
            ok = _true;
        }
        break;
    default:
        break;
    }
    if (ok && _opt.ml->u)
    { /* SUID */
        ok = S_ISUID && *mode;
    }
    if (ok && _opt.ml->g)
    { /* SGID */
        ok = S_ISGID && *mode;
    }
    if (ok && _opt.ml->t)
    { /* sticky bit */
        ok = S_ISVTX && *mode;
    }
    if (ok && _opt.ml->_1)
    { /* read by owner */
        ok = S_IRUSR & *mode;
    }
    if (ok && _opt.ml->_2)
    { /* write by owner */
        ok = S_IWUSR & *mode;
    }
    if (ok && _opt.ml->_3)
    { /* execute by owner */
        ok = S_IXUSR & *mode;
    }
    if (ok && _opt.ml->_4)
    { /* read by group */
        ok = S_IRGRP & *mode;
    }
    if (ok && _opt.ml->_5)
    { /* write by group */
        ok = S_IWGRP & *mode;
    }
    if (ok && _opt.ml->_6)
    { /* execute by group */
        ok = S_IXGRP & *mode;
    }
    if (ok && _opt.ml->_7)
    { /* read by others */
        ok = S_IROTH & *mode;
    }
    if (ok && _opt.ml->_8)
    { /* write by others */
        ok = S_IWOTH & *mode;
    }
    if (ok && _opt.ml->_9)
    { /* execute by others */
        ok = S_IXOTH & *mode;
    }
    return ok;
}
void core(_tree_info *tree)
{
    DIR *dir = opendir(_path);
    struct dirent *item;
    struct stat s;
    linklist files_list = lopen();
    _file file;
    long int cnt = 0;
    _bool ok, err_occured;

    // keep value of "path" and "path_z"
    if (!dir)
    {
        strcpy(_buffer, strerror(errno));
        if (_opt.t)
        {
            tree_display(tree, _true);
            printf("access denied: %s\n", _buffer);
        }
        else
        {
            printf("%s: access denied to \"%s\": %s\n", PROGRAM, _path, _buffer);
        }
        return;
    }

    while ((item = readdir(dir)))
    {
        ok = _false;
        err_occured = _true;
        if (_opt.ml->h || (item->d_name[0] != '.'))
        {
            strcpy(&_path[_path_len], item->d_name);
            if (_stat(_path, &s))
            {
                if (validate_mode(&s.st_mode))
                {
                    ok = _true;
                    err_occured = _false;
                }
            }
            if (ok)
            {
                if (_opt.c)
                {
                    ++cnt;
                }
                else
                {
                    file = (_file)_alloc(_FILE_SIZE);
                    file->name = (char *)_alloc((strlen(item->d_name) + 1));
                    file->st = s;
                    file->err = err_occured;
                    strcpy(file->name, item->d_name);
                    ladd(files_list, LLAST, file);
                }
            }
        }
    }
    closedir(dir);
    if (_opt.c)
    {
        printf("%ld\n", cnt);
        return;
    }
    if (lempty(files_list))
    {
        return;
    }
    if (_opt.s_char != 'd')
    {
        _sort(files_list);
    }
    if (_opt.t)
    { // format tree
        tree_main(files_list, tree);
    }
    else if (_opt.l)
    { // format long
        long_main(files_list);
    }
    else if (_opt._1 || _opt._2 || _opt._3 || _opt._4)
    { // format long
        list_main(files_list, NULL, 0);
    }
    else
    { // format column
        column_main(files_list, NULL, 0);
    }
    for (iterator it = lat(files_list, LFIRST); it; linc(&it))
    {
        file = (_file)it->data;
        free(file->name);
        free(file);
    }
    lclose(files_list);
}

int _sort_name(_file f1, _file f2)
{
    return _strcmp(f1->name, f2->name);
}
int _sort_i(_file f1, _file f2)
{
    return (f1->st.st_ino < f2->st.st_ino) ? -1 : 1;
}
int _sort_n(_file f1, _file f2)
{
    return (f1->st.st_nlink < f2->st.st_nlink) ? -1 : 1;
}
int _sort_u(_file f1, _file f2)
{
    return (f1->st.st_uid < f2->st.st_uid) ? -1 : 1;
}
int _sort_g(_file f1, _file f2)
{
    return (f1->st.st_gid < f2->st.st_gid) ? -1 : 1;
}
int _sort_s(_file f1, _file f2)
{
    return (f1->st.st_size < f2->st.st_size) ? -1 : 1;
}
int _sort_a(_file f1, _file f2)
{
    return (f1->st.st_atime < f2->st.st_atime) ? -1 : 1;
}
int _sort_m(_file f1, _file f2)
{
    return (f1->st.st_mtime < f2->st.st_mtime) ? -1 : 1;
}
int _sort_c(_file f1, _file f2)
{
    return (f1->st.st_ctime < f2->st.st_ctime) ? -1 : 1;
}
int _sort_t(_file f1, _file f2)
{
    return ((f1->st.st_mode) < (f2->st.st_mode)) ? -1 : 1;
}
int _sort_e(_file f1, _file f2)
{
    char *s1 = file_ext(f1->name), *s2 = file_ext(f2->name);
    if (s1 && s2 && strcmp(s1, s2))
    {
        return _strcmp(s1, s2);
    }
    return _strcmp(f1->name, f2->name);
}

void _sort(linklist l)
{
    lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_name);

    switch (_opt.s_char)
    {
    case 'i':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_i);
        break;

    case 'n':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_n);
        break;

    case 'u':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_u);
        break;

    case 'g':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_g);
        break;

    case 's':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_s);
        break;

    case 'a':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_a);
        break;

    case 'm':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_m);
        break;

    case 'c':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_c);
        break;

    case 't':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_t);
        break;

    case 'e':
        lsort(l, LFIRST, LLAST, (int (*)(void *, void *))_sort_e);
        break;
    default:
        break;
    }
}
