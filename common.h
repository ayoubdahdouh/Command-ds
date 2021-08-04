#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <sys/stat.h>
#include "list.h"
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct lfoptions
{
    int a; // all, by default lf doesn't show hidden file.
    int c; // for a colorful output.
    int d; // show only directories.
    int f; // show only files.
    int g; // show groups of files.
    int l; // show file's informations(permissions and size).
    int m; // show modification time of each file.
    int p; // show permissions of files.
    int h; // used with -s, make the size readable like 4K, 13M, 2G, etc
    int s; // show sizes of files.
    int t; // recursive(tree), by default lf shows current folder.
    int u; // show the owners of files.
    int version;
    int help;
} lfoptions;

typedef struct lftype
{
    char *name;
    struct stat st;

} * lftype;

#define OPTIONSIZ sizeof(struct lfoptions)
#define LFSIZ sizeof(struct lftype)

// #define RST "\033[0m"
// #define BLUE "\033[1;34m"                 /*dirctory*/
// #define CYAN "\033[1;36m"                 /*symbolic link*/
// #define YELLOW_BLACK "\033[40;33"         /*pipe*/
// #define YELLOW_BLACK_BOLD "\033[40;33;01" /*block device, character device*/
// #define PINK "\033[01;35"                 /*socket, door*/
// #define RED "\033[1;31m"
// #define GREEN "\033[1;32m"
// #define YELLOW "\033[40;33;01"
// #define RED_BLACK "\033[40;31;01"
// #define RED_BLACK "\033[40;31;01"

extern lfoptions opt;
extern char *path, *buf;
extern int pathsiz, clr, fl;
extern LIST lcolor;

void help();
void version();

void *lf_alloc(long int size);
int lf_stat(const char *nm, struct stat *s);
int lf_link(const char *nm);
void lf_error(int e, char *msg, bool is_sys_err);
void lf_init();
void lf_quit();

int has_space(const char *nm);
int is_absolute_path(const char *pth);
char *lfext(char *pth);

#define ERR_INVALID_OPTION 1
#define ERR_COLORS_NOT_AVILABLE 2

#endif