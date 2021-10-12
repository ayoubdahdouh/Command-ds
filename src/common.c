#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include "common.h"
#include "linkedList.h"
#include <wchar.h>

void printHelp()
{
    printf("Usage:  %s [OPTION]:[ARGUMENT],... [FILE]...\n\n", PROGRAM);
    printf("    List information about the FILEs (the current directory by default).\n");
    printf("    it's possible to combine those options together\n\n");
    printf("    -1  separate files with a newline(\\n).\n");
    printf("    -2  separate files with a space (SPACE).\n");
    printf("    -3  separate files with commas(,).\n");
    printf("    -4  separate files with a semicolon(;).\n");
    printf("    -c  count number of files and folders.\n");
    printf("    -d  display directories/dirlinks themselves, not their contents.\n");
    printf("        by default, %s display directories/dirlinks contents.\n", PROGRAM);
    printf("    -[...]m:[hbcdplrsugt123456789]   Mode settings\n");
    printf("          choose the files to display from the display below:\n");
    printf("            h  hidden files, all files began with a dot.\n");
    printf("            b  block device\n");
    printf("            c  character device\n");
    printf("            d  directory\n");
    printf("            p  FIFO/pipe\n");
    printf("            l  symlink\n");
    printf("            r  regular file\n");
    printf("            s  socket\n");
    printf("            u  SUID (set-user identification)\n");
    printf("            g  SGID (set-group identification)\n");
    printf("            t  sticky bit\n");
    printf("            1  read by owner\n");
    printf("            2  write by owner\n");
    printf("            3  execute by owner\n");
    printf("            4  read by group\n");
    printf("            5  write by group\n");
    printf("            6  execute by group\n");
    printf("            7  read by others\n");
    printf("            8  write by others\n");
    printf("            9  execute by others\n");
    printf("          Without '-m', %s displays all files and folders except the hidden ones.\n", PROGRAM);
    printf("          By default, '-m' displays all files and folders.\n");
    printf("          wich has the same effect as not using the '-m' option.\n");
    printf("    -[...]t:[DEPTH]   Tree settings\n");
    printf("            DEPTH  tree depth\n");
    printf("          By default (without DEPTH), the depth is unlimited.\n");
    printf("    -[...]l:[inpsrugamc]   Information settings\n");
    printf("          choose the information to display from the display below:\n");
    printf("            i  inode number\n");
    printf("            n  number of hard links\n");
    printf("            p  permissions\n");
    printf("            s  size in byte\n");
    printf("            r  readable size like 5K, 43M, 1.7G etc.\n");
    printf("            u  file owner (user)\n");
    printf("            g  file group\n");
    printf("            a  last access\n");
    printf("            m  last modification\n");
    printf("            c  last status change\n");
    printf("          By default, if no argument is set, '-l' show i,n,p,r,u,g,m.\n");
    printf("    -[...]n:[cbfqi]   Name settings\n");
    printf("            c  color the output\n");
    printf("            f  follow link, if the file is link then display also link content\n");
    printf("            b  adds a backslash to the string containing spaces\n");
    printf("            q  adds quotes to the string containing spaces\n");
    printf("            i  adds a character indicator to filename\n");
    printf("                '*'  executable files\n");
    printf("                '/'  directories\n");
    printf("                '@'  symbolic links\n");
    printf("                '|'  FIFO/pipe\n");
    printf("                '='  sockets\n");
    printf("          By default, if no argument is set, '-n' show c,f,q.\n");
    printf("    -[...]s:[dinsugamcte]   Sort settings\n");
    printf("          Sort the output\n");
    printf("            i  inode number\n");
    printf("            n  number of hard links\n");
    printf("            s  size\n");
    printf("            u  file owner\n");
    printf("            g  file group\n");
    printf("            a  last access\n");
    printf("            m  last [m]odification\n");
    printf("            c  last status [c]hange\n");
    printf("            t  file [t]ype\n");
    printf("            e  file [e]xtension\n");
    printf("          By default, if no argument is set, '-s' will sort the output by names,\n");
    printf("    -h  print help.\n");
    printf("    -v  print version information.\n");
}

void printVersion()
{
    printf("Version: %s %s.\n", PROGRAM, VERSION);
}

void *memAlloc(long int size)
{
    void *b;
    b = malloc(size);
    if (!b)
    {
        printf("%s: %s\n", PROGRAM, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return b;
}
// places the contents of the symbolic link pathname in the buffer "buf" (global variable).
// if failed, also print error in the buffer "buf"
Bool readLink(const char *nm)
{
    int cnt;
    if (!nm)
    {
        return False;
    }
    cnt = readlink(nm, Bfr, PATH_MAX - 1);
    if (cnt == -1)
    {
        strcpy(Bfr, strerror(errno));
        return False;
    }
    Bfr[cnt] = 0;
    return True;
}
// if failed, also print error in the buffer "buf"
Bool fileStat(const char *nm, struct stat *s)
{
    if (lstat(nm, s) == -1)
    {
        return False;
    }
    return True;
}

void initProgram()
{
    setbuf(stdout, NULL);
    // init PATH
    Pth = (char *)memAlloc(PATH_MAX);
    // init BUF
    Bfr = (char *)memAlloc(PATH_MAX);
}

void exitProgram(char *msg)
{
    if (Pth)
    {
        free(Pth);
    }
    if (Bfr)
    {
        free(Bfr);
    }
    if (msg)
    {
        printf("%s\n", msg);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int countSpaces(char *nm)
{
    int i = 0;
    for (; *nm; ++nm)
    {
        if (*nm == ' ')
        {
            ++i;
        }
    }
    return i;
}

Bool isAbsolutePath(const char *p)
{
    return (p) ? (p[0] == '/') : False;
}

char *fileExtension(char *s)
{
    // not null and not empty
    char *last = s + strlen(s) - 1;
    if (s && *s)
    {
        char *c = last;
        while (c > s && *c != '.' && *c != '/')
        {
            --c;
        }
        if (*c == '.')
        {
            // c!=s otherwise "s" is hidden file/folder ex: .bashrc
            // c!=last otherwise "s" have ho exention ex: hello.
            // *(c - 1)!='/' otherwise "s" is hidden file/folder ex: /home/user/.bashrc
            if ((c != s) && (c != last) && (*(c - 1) != '/'))
            {
                return c;
            }
        }
    }
    return NULL;
}

char fileType(mode_t *m)
{
    char c;
    switch (*m & S_IFMT)
    {
    case S_IFBLK:
        c = 'b';
        break;
    case S_IFCHR:
        c = 'c';
        break;
    case S_IFDIR:
        c = 'd';
        break;
    case S_IFIFO:
        c = 'p';
        break;
    case S_IFLNK:
        c = 'l';
        break;
    case S_IFREG:
        c = '~';
        break;
    case S_IFSOCK:
        c = 's';
        break;
    default:
        c = 0;
        break;
    }
    return c;
}

int strWidth(char *s)
{
    int len;
    wchar_t *wcs;

    // length of multibyte characters
    len = mbstowcs(NULL, s, 0) + 1;
    wcs = malloc(len * sizeof(wchar_t));
    // convert to wide characters
    mbstowcs(wcs, s, len);
    // display width
    return wcswidth(wcs, len * sizeof(wchar_t));
}

// strings compareing no case sensative
int strCompare(char *s1, char *s2)
{
    int res = 0;
    char c1, c2;

    if (!s1 && !s2)
    {
        return 0;
    }
    else if (!s1)
    {
        return -1;
    }
    else if (!s2)
    {
        return 1;
    }
    while (!res && *s1 && *s2)
    {
        // convert s1 to lower case
        c1 = *s1 + ((*s1 >= 'A' && *s1 <= 'Z') ? 32 : 0);
        // convert s2 to lower case
        c2 = *s2 + ((*s2 >= 'A' && *s2 <= 'Z') ? 32 : 0);

        if (c1 < c2)
        {
            res = -1;
        }
        else if (c1 > c2)
        {
            res = 1;
        }
        else
        {
            ++s1;
            ++s2;
        }
    }
    if (!res)
    {
        if (*s1)
        {
            return 1;
        }
        return -1;
    }
    return res;
}

// count number of bit equal to 1.
int countActiveBits(u_int32_t stream, int n)
{
    int cnt = 0;

    for (int i = 0; i < n; i++)
    {
        if (stream & (1 << i))
        {
            ++cnt;
        }
    }
    return cnt;
}