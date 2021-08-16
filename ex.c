#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *fileextension(char *tmp)
{
    if (tmp && *tmp)
    {
        char *c = tmp + strlen(tmp) - 1;

        while (c >= tmp && *c != '.' && *c != '/')
        {
            --c;
        }
        if (*c == '.')
        {
            return c;
        }
    }
    return NULL;
}

int main()
{
    struct stat s;
    char buf[1000];
    int n;
    char *s = fileextension("mlkj.txt");
    printf("%s\n", s);
    printf("%s\n", fileextension(".txt"));
    printf("%s\n", fileextension("txt"));
    printf("%s\n", fileextension(""));
    printf("%s\n", fileextension(NULL));

    return 0;
}