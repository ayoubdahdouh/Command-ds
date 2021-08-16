#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"
#include "common.h"

void run(linklist arguments);
void run();
void core(format_tree_t *tree);
int stringcmp(char *s1, char *s2);
void sort_tb(char **tb, int n);

void sort(linklist l);
int sort_name(lftype t1, lftype t2);
int sort_i(lftype t1, lftype t2);
int sort_n(lftype t1, lftype t2);
int sort_u(lftype t1, lftype t2);
int sort_g(lftype t1, lftype t2);
int sort_s(lftype t1, lftype t2);
int sort_a(lftype t1, lftype t2);
int sort_m(lftype t1, lftype t2);
int sort_c(lftype t1, lftype t2);
int sort_t(lftype t1, lftype t2);
int sort_e(lftype t1, lftype t2);

#endif