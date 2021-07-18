#ifndef FORMAT_LONG
#define FORMAT_LONG 1

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
    struct stat *st;
} format_long_t;

// S_ISVTX
void long_selector(char *fname, int max);
void long_display(char **tb, format_long_t *lf_tb, int tb_len, int max_user, int max_group, int max_size, int max_perm);
void long_main(char **tb, int tb_f, int tb_d, int tb_len);
char *long_permission(char *perm, __mode_t *mode);
char *long_size(char *buf, long int size2);
char *long_user(char *user, uid_t uid);
char *long_group(char *group, gid_t gid);
char *long_mtime(char *mtime, const time_t *atm);

#endif