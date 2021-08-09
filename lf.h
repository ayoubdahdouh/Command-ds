#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"
#include "common.h"

void run(linklist arguments);
void run();
void core(format_tree_t *tree);
int cmp(lftype t1, lftype t2);
void sort_tb(char **tb, int n);

#endif