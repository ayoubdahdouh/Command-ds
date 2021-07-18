#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"

void run(list arguments);
void initial();
void finish();
void run();
void core(format_tree_t *tree);
int str_cmp(char *s1, char *s2);
void sort_tb(char **tb, int n);

#endif