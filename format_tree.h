#ifndef FORMAT_TREE
#define FORMAT_TREE 1

#include <stdbool.h>
#include "list.h"

typedef struct format_tree_t
{
    int level;
    char *parent_has_next;
} format_tree_t;


#define LFTREESIZ sizeof(format_tree_t)
void tree_display(format_tree_t *tree, bool last);
void tree_main(linklist l, int index, format_tree_t *tree);

#endif