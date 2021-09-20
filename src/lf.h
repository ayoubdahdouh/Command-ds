#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"
#include "common.h"

void core(_tree_info *tree);
int _strcmp(char *s1, char *s2);

void _sort(linklist l);
int _sort_name(_file t1, _file t2);
int _sort_i(_file t1, _file t2);
int _sort_n(_file t1, _file t2);
int _sort_u(_file t1, _file t2);
int _sort_g(_file t1, _file t2);
int _sort_s(_file t1, _file t2);
int _sort_a(_file t1, _file t2);
int _sort_m(_file t1, _file t2);
int _sort_c(_file t1, _file t2);
int _sort_t(_file t1, _file t2);
int _sort_e(_file t1, _file t2);

#endif