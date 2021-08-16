#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <sys/stat.h>
#include "list.h"
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct m_option
{
    bool b, c, d, p, l, f, s, u, g, t, r, w, x;
} m_option;
#define MOPTIONSIZ sizeof(struct m_option)

typedef struct l_option
{
    bool i, n, u, g, s, p, a, m, c;
} l_option;
#define LOPTIONSIZ sizeof(struct l_option)

// typedef struct s_option
// {
//     bool i, n, u, g, s, p, a, m, c, t, e;
// } s_option;
// #define SOPTIONSIZ sizeof(struct s_option)

typedef struct lfoption
{
    bool a; // all, by default lf doesn't show hidden file.
    bool m; // mode.
    m_option *ml; // mode list

    bool l; // format
    l_option *ll;

    bool r; // make the size readable like 4K, 13M, 2G, etc
    bool s; // sort by i/n/u/g/s/a/m/c/d/r/b/type/extension
    char sc; // s character

    bool t; // tree format
    long td; // tree depth

    bool w;
    char* ws;
    bool c; // for a colorful output.

    bool v;
    bool h;
} lfoption;

typedef struct lftype
{
    char *name;
    struct stat st;

} * lftype;

#define OPTIONSIZ sizeof(struct lfoption)
#define LFSIZ sizeof(struct lftype)

extern lfoption LFopt;
extern char *LFpath, *LFbuf;
extern int LFpathsiz;
extern bool LF_use_color, LF_follow_link;
extern linklist LFcolorlist;

void help(char h);
void version();

void *lf_alloc(long int size);
bool lf_stat(const char *nm, struct stat *s);
bool lf_link(const char *nm);
void lf_init();
void lf_quit();

int has_space(const char *nm);
bool is_absolute_path(const char *pth);
char *fileextension(char *s);
char filetype(mode_t *m);

#define ERR_INVALID_OPTION -1001
#define ERR_COLORS_NOT_AVILABLE -1002
#define ERR_DEPTH_WRONG -1003

#endif