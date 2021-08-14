#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    struct stat s;
    char buf[1000];
    int n;

    if (lstat("salut", &s) != -1)
    {
        printf("salut link? %d", S_ISLNK(s.st_mode & S_IFMT));
        if (S_ISLNK(s.st_mode & S_IFMT))
        {
            n = readlink("salut", buf, 1000);
            if (n != -1)
            {
                buf[n] = 0;
                printf(" ;%s\n", buf);
            }
            else
            {
                printf("\n");
            }
        }
    }
    if (lstat("salut2", &s) != -1)
    {
        printf("salut2 link? %d", S_ISLNK(s.st_mode & S_IFMT));
        if (S_ISLNK(s.st_mode & S_IFMT))
        {
            n = readlink("salut2", buf, 1000);
            if (n != -1)
            {
                buf[n] = 0;
                printf(" ;%s\n", buf);
            }
            else
            {
                printf("\n");
            }
        }
    }

    return 0;
}