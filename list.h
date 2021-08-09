#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct node
{
    void *data;
    struct node *next;
} * node, *iterator;

typedef struct linklist
{
    int count; // number of elements in LIST
    node first;
    node last;
} * linklist;

#define LINKLISTSIZ sizeof(struct linklist)
#define NODESIZ sizeof(struct node)
#define ITERATORSIZ NODESIZ
#define LLAST -1
#define LFIRST -2

// list operations
linklist lopen();
void lclose(linklist l);

int lempty(linklist l);  // if it's empty
void lreset(linklist l); // empty listI

void ladd(linklist l, long int at, void *data);
void ldel(linklist l, long int at);

void *lget(linklist l, long int at);
void *lset(linklist l, long int at, void *data);

void lquicksort(linklist l, long int from, long int to, int selector(void *, void *));

// show list
void lshow(linklist l);

//  iterator functions
iterator lat(linklist l, int at);
void linc(iterator *i);

#endif