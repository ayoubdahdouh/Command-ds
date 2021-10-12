#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct Node
{
    void *data;
    struct Node *next;
} * Node, *Iterator;

typedef struct LinkedList
{
    long int count; // number of elements in LIST
    Node first;
    Node last;
} * LinkedList;

#define LINKEDLIST_SIZE sizeof(struct LinkedList)
#define NODE_SIZE sizeof(struct Node)
#define ITERATOR_SIZE NODESIZ
#define LLAST -1
#define LFIRST -2

// LinkedList operations
LinkedList lOpen();
void lClose(LinkedList l);

int lEmpty(LinkedList l);  // if it's empty
void lReset(LinkedList l); // empty listI

void lInsert(LinkedList l, long int at, void *data);
void lDelete(LinkedList l, long int at);

void *lGet(LinkedList l, long int at);
void *lSet(LinkedList l, long int at, void *data);

void lReverse(LinkedList l);

void lSort(LinkedList l, long int from, long int to, int selector(void *, void *));

//  iterator functions
Iterator lAt(LinkedList l, int at);
void lInc(Iterator *i);

#endif