#ifndef FORMAT_LONG
#define FORMAT_LONG 1

#include <stdbool.h>
#include "list.h"

typedef struct format_long_t
{
    char **user;
    char **group;
    char **size;
    char **perm;
    char **mtime;
    /**
     * perhaps we will add ctime, atime.
     * */ 
} format_long_t;

#define FORMATLONGSIZ sizeof(struct format_long_t)

// S_ISVTX
void long_print(char *fname, int max, int flag);
void long_display(LIST l, format_long_t *lf_tb, int max_user, int max_group, int max_size, int max_perm);
void long_main(LIST l);
char *long_permission(char *perm, __mode_t *mode);
char *long_size(char *buf, long int size2);
char *long_user(char *user, uid_t uid);
char *long_group(char *group, gid_t gid);
char *long_mtime(char *mtime, const time_t *atm);

#endif