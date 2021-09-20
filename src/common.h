#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#include "list.h"
#include "types.h"

#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

extern _options _opt;
extern char *_path, *_buffer;
extern int _path_len;
extern linklist _colors_list;
extern char *_time_style;

void _print_help();
void _print_version();

void *_alloc(long int size);
_bool _stat(const char *nm, struct stat *s);
_bool _link(const char *nm);

void _initial();
void _quit(char *msg); // free memories + print "msg" if exist and return 0 otherwise return 1

int has_space(char *nm);
_bool is_absolute_path(const char *pth);
char *file_ext(char *s);
char file_type(mode_t *m);

// display width of the string, for non-ASCII characters
int str_width(char *s);

int _strcmp(char *s1, char *s2);

#define ERR_INVALID_OPTION -1001
#define ERR_COLORS_NOT_AVILABLE -1002
#define ERR_DEPTH_WRONG -1003

#endif