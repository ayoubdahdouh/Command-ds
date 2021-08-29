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
int has_space(char *name)
{
    int i = 0;
    for (; *name; ++name)
    {
        if (*name == ' ')
        {
            ++i;
        }
    }
    return i;
}
int main()
{

    for (int i = 0; i < 300; i++)
    {
        putchar('a');
    }
    putchar('\n');

    return 0;
}