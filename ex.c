#include <time.h>

int main()
{
    struct tm *t;
    time_t tt;

    tt = time(NULL);
    t = localtime(&tt);
    return 0;
}