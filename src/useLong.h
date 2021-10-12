#ifndef FORMAT_LONG
#define FORMAT_LONG 1

#include "linkedList.h"
#include "types.h"

void printFormattedValue(char *nm, int m, Bool rtl);
void longDisplay(linkedList l, FileInfo *filesInfo, int max[]);
void longMain(linkedList l);
int nbrOfDigits(long int n);
char *getIno(char *buf, ino_t data);
char *getNlink(char *buf, nlink_t data);
char *getSize(char *buf, long int data);
char *getReadableSize(char *buf, long int data);
char *getUser(char *user, uid_t uid);
char *getGroup(char *group, gid_t gid);
char *getTime(char *buf, const time_t *atm);
char *getPermissions(char *b, __mode_t *m);

#endif