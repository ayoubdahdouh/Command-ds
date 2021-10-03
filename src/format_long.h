#ifndef FORMAT_LONG
#define FORMAT_LONG 1

#include "list.h"
#include "types.h"

void long_print(char *nm, int m, _bool rtl);
void long_display(linklist l, _file_info *files_info, int mi, int mn, int mu, int mg, int mp, int ms, int ma, int mm, int mc);
void _free_tb(char **tb, int n);
void long_main(linklist l);
int number_size(long int n);
char *long_ino(char *buf, ino_t data);
char *long_nlink(char *buf, nlink_t data);
char *long_size(char *buf, long int data);
char *long_size_readable(char *buf, long int data);
char *long_user(char *user, uid_t uid);
char *long_group(char *group, gid_t gid);
char *long_time(char *buf, const time_t *atm);
char *long_permission(char *b, __mode_t *m);

#endif