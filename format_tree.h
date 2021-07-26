#ifndef FORMAT_TREE
#define FORMAT_TREE 1

#include "list.h"

typedef struct format_tree_t
{
    int level;
    char *parent_has_next;
} format_tree_t;

void tree_display(format_tree_t *tree, int last);
void tree_main(LIST l, int index, format_tree_t *tree);

#endif