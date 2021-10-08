#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct Node
{
    void *data;
    struct Node *next;
} * Node, *Iterator;

typedef struct linkedList
{
    long int count; // number of elements in LIST
    Node first;
    Node last;
} * linkedList;

#define LINKEDLIST_SIZE sizeof(struct linkedList)
#define NODE_SIZE sizeof(struct Node)
#define ITERATOR_SIZE NODESIZ
#define LLAST -1
#define LFIRST -2

// linkedList operations
linkedList lOpen();
void lClose(linkedList l);

int lEmpty(linkedList l);  // if it's empty
void lReset(linkedList l); // empty listI

void lInsert(linkedList l, long int at, void *data);
void lDelete(linkedList l, long int at);

void *lGet(linkedList l, long int at);
void *lSet(linkedList l, long int at, void *data);

void lReverse(linkedList l);

void lSort(linkedList l, long int from, long int to, int selector(void *, void *));

//  iterator functions
Iterator lAt(linkedList l, int at);
void lInc(Iterator *i);

#endif