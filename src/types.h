#ifndef TYPES_H
#define TYPES_H

#include <sys/stat.h>

typedef enum
{
    _false,
    _true
} _bool;

typedef struct _arg_m
{
    _bool h;
    _bool b;
    _bool c;
    _bool d;
    _bool p;
    _bool l;
    _bool r;
    _bool s;
    _bool u;
    _bool g;
    _bool t;
    _bool _1;
    _bool _2;
    _bool _3;
    _bool _4;
    _bool _5;
    _bool _6;
    _bool _7;
    _bool _8;
    _bool _9;
} _arg_m;
#define _ARG_M_SIZE sizeof(_arg_m)

typedef struct _arg_l
{
    _bool i;
    _bool n;
    _bool u;
    _bool g;
    _bool s;
    _bool r;
    _bool p;
    _bool a;
    _bool m;
    _bool c;
} _arg_l;
#define _ARG_L_SIZE sizeof(_arg_l)

typedef struct _arg_n
{
    _bool c;
    _bool b;
    _bool f;
    _bool q;
    _bool i; /* character indicator */
} _arg_n;
#define _ARG_N_SIZE sizeof(_arg_n)

typedef struct _options
{
    _bool _1;
    _bool _2;
    _bool _3;
    _bool _4;
    _bool d;    // manipulate argument itself not (its content if it's link) or not its reference if it's link
    _bool m;    // mode
    _arg_m *ml; // mode list
    _bool l;    // information
    _arg_l *ll;
    char s_char; // sort character
    _bool t;     // tree format
    long td;     // tree depth
    _bool n;
    _arg_n *nl;
    _bool v;
    _bool h;
} _options;
#define _OPTIONS_SIZE sizeof(_options)

typedef struct _file
{
    char *name;
    struct stat st;
    int err;

} * _file;
#define _FILE_SIZE sizeof(struct _file)

#define I_INDEX 0
#define N_INDEX 1
#define U_INDEX 2
#define G_INDEX 3
#define S_INDEX 4
#define P_INDEX 5
#define A_INDEX 6
#define M_INDEX 7
#define C_INDEX 8
typedef struct _file_info
{
    char *bfr[9];
} _file_info;

#define _FILE_INFO_SIZE sizeof(struct _file_info)

typedef struct _tree_info
{
    int level;
    char *has_next;
} _tree_info;
#define _TREE_INFO_SIZE sizeof(_tree_info)
#define MAX_DEPTH (PATH_MAX - 1)

typedef struct _colors
{
    char *a;
    char *c;
    _bool e;
} _colors;

#define COLORS_SIZE sizeof(_colors)

#endif