#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#include "linkedList.h"
#include "types.h"

#define PROGRAM "ds"
#define VERSION "1.0.0"

extern u_int8_t Nparams;
extern u_int16_t Sparams;
extern u_int16_t Lparams;
extern u_int32_t Mparams;
extern u_int16_t Opts;
extern int Tparam;
extern char *Pth, *Bfr;
extern int PthLen;
extern LinkedList ColorsList;
extern char *TimeStyle;

void printHelp();
void printVersion();

void *memAlloc(long int size);
Bool fileStat(const char *nm, struct stat *s);
Bool readLink(const char *nm);

void initProgram();
void exitProgram(char *msg); // free memories + print "msg" if exist and return 0 otherwise return 1

int countSpaces(char *nm);
Bool isAbsolutePath(const char *pth);
char *fileExtension(char *s);
char fileType(mode_t *m);

// display width of the string, for non-ASCII characters
int strWidth(char *s);

int strCompare(char *s1, char *s2);

int countActiveBits(u_int32_t stream, int n);


#endif