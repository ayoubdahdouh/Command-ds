#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct _l_node
{
    void *data;
    struct _l_node *next;
} * l_node;

typedef struct _list
{
    int size;  // data size.
    int count; // number of elements in list
    l_node top;
    l_node bottom;
} * list;

#define LIST_SIZE sizeof(struct _list)
#define L_NODE_SIZE sizeof(struct _l_node)
#define L_POINTER 0


list l_open(long int data_size);
void l_close(list lst);

int l_empty(list lst);

void *l_top(list lst);
void *l_bot(list lst);
void *l_at(list lst, int i);

void l_add_top(list lst, void *d);
void l_add_bot(list lst, void *d);
void l_del_top(list lst);
void l_del_bot(list lst);
void l_del_at(list lst, int i);


#endif