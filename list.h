#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct NODE
{
    void *data;
    struct NODE *next;
} * NODE, *ITERATOR;

typedef struct LIST
{
    int count; // number of elements in LIST
    NODE first;
    NODE last;
} * LIST;

#define LISTSIZ sizeof(struct LIST)
#define NODESIZ sizeof(struct NODE)
#define ITERATORSIZ NODESIZ
#define LLAST -1
#define LFIRST -2

// list operations
LIST LOPEN();
void LCLOSE(LIST l);

int LEMPTY(LIST l);  // if it's empty
void LRESET(LIST l); // empty listI

void LADD(LIST l, long int at, void *data);
void LDEL(LIST l, long int at);

void *LGET(LIST l, long int at);
void *LSET(LIST l, long int at, void *data);

void LSORT(LIST l, long int from, long int to, int selector(void *, void *));

// show list
void LSHOW(LIST l);

//  iterator functions
ITERATOR LAT(LIST l, int at);
void LINC(ITERATOR *i);

#endif