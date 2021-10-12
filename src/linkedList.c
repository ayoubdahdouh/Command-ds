#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

void quickSort(linkedList l, int from, int to, int compare(void *, void *));
int partition(linkedList l, int from, int to, int compare(void *, void *));
void reverseOrder(Node curr, Node next);

void LERR(const char *msg)
{
    if (msg)
    {
        printf("%s\n", msg);
    }
}

linkedList lOpen()
{
    linkedList l;

    l = (linkedList)malloc(LINKEDLIST_SIZE);
    if (!l)
    {
        LERR("[LOPEN] Memory allocation failed");
        return NULL;
    }
    l->count = 0;
    l->first = l->last = NULL;

    return l;
}

void lClose(linkedList l)
{
    if (!l)
    {
        Node x, y = l->first;
        while (l->count)
        {
            --l->count;
            x = y;
            y = y->next;
            free(x);
        }
        free(l);
    }
}

int lEmpty(linkedList l)
{
    if (l)
    {
        return (l->count == 0);
    }
    return 1;
}

void lReset(linkedList l)
{
    if (!l)
    {
        LERR("[LRESET] No linkedList is given");
        return;
    }
    while (!lEmpty(l))
    {
        lDelete(l, LFIRST);
    }
}

void lInsert(linkedList l, long int at, void *data)
{
    Node n, x, y;

    if (!l)
    {
        LERR("[lInsert] No linkedList is given");
        return;
    }
    if (!data)
    {
        LERR("[lInsert] No data is given");
        return;
    }
    if (at < LFIRST || at > l->count)
    {
        LERR("[lInsert] Out of bounds");
        return;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count;
    }
    n = (Node)malloc(NODE_SIZE);
    n->data = data;
    n->next = NULL;

    if (l->count == 0)
    {
        l->first = n;
    }
    else if (l->count == 1)
    {
        if (at == 0)
        {
            n->next = l->first;
            l->last = l->first;
            l->first = n;
        }
        else
        {
            l->first->next = n;
            l->last = n;
        }
    }
    else
    {
        if (at == 0)
        {
            n->next = l->first;
            l->first = n;
        }
        else if (at == l->count)
        {
            l->last->next = n;
            l->last = n;
        }
        else
        {
            x = l->first;
            while (at)
            {
                --at;
                y = x;
                x = x->next;
            }
            n->next = x;
            y->next = n;
        }
    }
    l->count++;
}

void lDelete(linkedList l, long int at)
{
    Node x, y;

    if (!l)
    {
        LERR("[LDEL] No linkedList is given");
        return;
    }
    if (at < LFIRST || at >= l->count)
    {
        LERR("[LDEL] Out of bounds");
        return;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    if (l->count == 0)
    { // if LAST's empty
        return;
    }
    else if (l->count == 1)
    {
        x = l->first;
        l->first = NULL;
    }
    else if (l->count == 2)
    {
        if (at == 0)
        {
            x = l->first;
            l->first = l->last;
            l->last = NULL;
        }
        else
        {
            x = l->last;
            l->first->next = NULL;
            l->last = NULL;
        }
    }
    else
    {
        if (at == 0)
        {
            x = l->first;
            l->first = x->next;
        }
        else
        {
            x = l->first;
            while (at)
            {
                --at;
                y = x;
                x = x->next;
            }
            y->next = x->next;
        }
    }
    free(x);
    --l->count;
}

void *lGet(linkedList l, long int at)
{
    Node y;

    if (!l)
    {
        LERR("[LGET] No linkedList is given");
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        LERR("[LGET] Out of bounds");
        return NULL;
    }
    if (lEmpty(l))
    {
        LERR("[LGET] The linkedList is empty");
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    y = l->first;
    while (at)
    {
        --at;
        y = y->next;
    }
    return y->data;
}

void *lSet(linkedList l, long int at, void *data)
{
    Node n;
    void *tmp;

    if (!l)
    {
        LERR("[LSET] No linkedList is given");
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        LERR("[LSET] Out of bounds");
        return NULL;
    }
    if (lEmpty(l))
    {
        LERR("[LSET] The linkedList is empty");
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    n = l->first;
    while (at)
    {
        --at;
        n = n->next;
    }
    tmp = n->data;
    n->data = data;
    return tmp;
}

void lSort(linkedList l, long int from, long int to, int compare(void *, void *))
{
    if (!l)
    {
        return;
    }
    if ((from < LFIRST || (from >= l->count)) &&
        (to < LFIRST || (to >= l->count)))
    {
        LERR("[LSORT] Out of bounds");
        return;
    }
    if (from == LFIRST)
    {
        from = 0;
    }
    else if (from == LLAST)
    {
        from = l->count - 1;
    }
    if (to == LFIRST)
    {
        to = 0;
    }
    else if (to == LLAST)
    {
        to = l->count - 1;
    }
    if (from > to)
    {
        LERR("[LSORT] 'from' must be less than 'to'");
        return;
    }
    quickSort(l, from, to, compare);
}

int partition(linkedList l, int from, int to, int compare(void *, void *))
{
    Iterator p = lAt(l, to);
    Iterator j = lAt(l, from);
    Iterator i = NULL;
    void *x;
    int k = from;

    while (to > from)
    {
        if (compare(j->data, p->data) < 0)
        {
            if (i)
            {
                lInc(&i);
            }
            else
            {
                i = lAt(l, from);
            }
            x = j->data;
            j->data = i->data;
            i->data = x;
            ++k;
        }
        lInc(&j);
        --to;
    }
    if (i)
    {
        lInc(&i);
    }
    else
    {
        i = lAt(l, from);
    }
    x = p->data;
    p->data = i->data;
    i->data = x;
    return k;
}

void quickSort(linkedList l, int from, int to, int compare(void *, void *))
{
    if (from < to)
    {
        int p = partition(l, from, to, compare);
        quickSort(l, from, p - 1, compare);
        quickSort(l, p + 1, to, compare);
    }
}

void lInc(Iterator *i)
{
    if (i)
    {
        *i = (*i)->next;
    }
}

Iterator lAt(linkedList l, int at)
{
    Iterator i;

    if (!l)
    {
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        return NULL;
    }
    if (lEmpty(l))
    {
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    i = l->first;
    while (at)
    {
        --at;
        i = i->next;
    }
    return i;
}

void reverseOrder(Node curr, Node next)
{
    if (next)
    {
        reverseOrder(next, next->next);
        next->next = curr;
    }
}
void lReverse(linkedList l)
{
    if (!l || (l->count <= 1))
    {
        return;
    }
    reverseOrder(l->first, l->first->next);
    l->first->next = NULL;
    Node tmp = l->last;
    l->last = l->first;
    l->first = tmp;
}

// void lShow(linkedList l)
// {
//     if (!l)
//     {
//         LERR("[lShow]: The linkedList is empty");
//     }
//     printf("%ld nodes: ", l->count);
//     for (int i = 0; i < l->count; i++)
//     {
//         printf("%s -> ", (char *)lGet(l, i));
//     }
//     printf("NULL\n\n");
// }

// int main()
// {
//     linkedList l = lOpen();
//     setbuf(stdout, NULL);

//     lInsert(l, LFIRST, "xaaa");
//     lInsert(l, LLAST, "abc");
//     lInsert(l, 2, "cba");
//     lInsert(l, 3, "jbA");
//     lInsert(l, LLAST, "ba");
//     lInsert(l, LLAST, "acba");
//     lShow(l);
//     lReverse(l);
//     lShow(l);
//     lClose(l);
// }
