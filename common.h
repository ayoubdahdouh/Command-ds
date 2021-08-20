#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <sys/stat.h>
#include "list.h"
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct m_arg
{
    bool b, c, d, p, l, f, s, u, g, t, r, w, x;
} m_arg;
#define M_ARG_SIZ sizeof(struct m_arg)

typedef struct i_arg
{
    bool i, l, u, g, s, p, a, m, c;
} i_arg;
#define L_ARG_SIZ sizeof(struct i_arg)

typedef struct n_arg
{
    bool f,q,s;
} n_arg;
#define N_ARG_SIZ sizeof(struct n_arg)

// typedef struct s_option
// {
//     bool i, n, u, g, s, p, a, m, c, t, e;
// } s_option;
// #define SOPTIONSIZ sizeof(struct s_option)

typedef struct lf_option
{
    bool arg;
    bool zero;
    bool one;
    bool two;
    bool a; // all, by default lf doesn't show hidden file.
    bool m; // mode.
    m_arg *ml; // mode list

    bool i; // information
    i_arg *il;

    bool r; // make the size readable like 4K, 13M, 2G, etc
    bool s; // sort output
    char sc; // s character

    bool t; // tree format
    long td; // tree depth

    bool n;
    n_arg* nl;

    bool c; // for a colorful output.

    bool v;
    bool h;
} lf_option;

typedef struct lf_type
{
    char *name;
    struct stat st;

} * lf_type;

#define OPTIONSIZ sizeof(struct lf_option)
#define LFSIZ sizeof(struct lf_type)

extern lf_option LFopt;
extern char *LFpath, *LFbuf;
extern int LFpathsiz;
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