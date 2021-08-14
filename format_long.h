#ifndef FORMAT_LONG
#define FORMAT_LONG 1

#include <stdbool.h>
#include "list.h"

typedef struct format_long_t
{
    char **u;
    char **g;
    char **s;
    char **p;
    char **m;
    char **i;
    char **n;
    /**
     * In next version we will add ctime, atime.
     * */ 
} format_long_t;

#define FORMATLONGSIZ sizeof(struct format_long_t)

// S_ISVTX
void long_print(char *fname, int max, int flag);
void long_display(linklist l, format_long_t *fl, int mp, int ms, int mu, int mg, int mi, int mn);
void long_main(linklist l);
char *long_permission(char *perm, __mode_t *mode);
char *long_size(char *buf, long int size2);
char *long_user(char *user, uid_t uid);
char *long_group(char *group, gid_t gid);
char *long_mtime(char *mtime, const time_t *atm);
char *long_ino(char *buf, ino_t data);
char *long_nlink(char *buf, nlink_t data);

#endif