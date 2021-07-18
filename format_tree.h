#ifndef FORMAT_TREE
#define FORMAT_TREE 1

typedef struct format_tree_t
{
    int level;
    char *parent_has_next;
} format_tree_t;

void tree_display(format_tree_t *tree, int last);
void tree_main(format_tree_t *tree, char **tb, int tb_f, int tb_d, int tb_len);

#endif