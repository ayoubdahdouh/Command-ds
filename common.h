#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <sys/stat.h>
#include "list.h"
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct lfoptions
{
    bool a; // all, by default lf doesn't show hidden file.
    bool c; // for a colorful output.
    bool d; // show only directories.
    bool f; // show only files.
    bool g; // show groups of files.
    bool l; // show file's informations(permissions and size).
    bool m; // show modification time of each file.
    bool p; // show permissions of files.
    bool h; // used with -s, make the size readable like 4K, 13M, 2G, etc
    bool s; // show sizes of files.
    bool t; // recursive(tree), by default lf shows current folder.
    long tdeep;
    bool u; // show the owners of files.
    bool version;
    bool help;
} lfoptions;

typedef struct lftype
{
    char *name;
    struct stat st;

} * lftype;

#define OPTIONSIZ sizeof(struct lfoptions)
#define LFSIZ sizeof(struct lftype)

extern lfoptions LF_opt;
extern char *LF_path, *LF_buf;
extern int LF_pathsiz;
extern bool LF_clr, LF_fl;
extern linklist LF_lcolor;

void help();
void version();

void *lf_alloc(long int size);
bool lf_stat(const char *nm, struct stat *s);
bool lf_link(const char *nm);
void lf_init();
void lf_quit();

int has_space(const char *nm);
bool is_absolute_path(const char *pth);
char *lfext(const char *pth);

char filetype(mode_t *m);

#define ERR_INVALID_OPTION -1001
#define ERR_COLORS_NOT_AVILABLE -1002
#define ERR_DEPTH_WRONG -1003

#endif