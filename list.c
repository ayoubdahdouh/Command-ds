#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

l_node l_create_node(long size);

void l_error(const char *msg);

void l_error(const char *msg)
{
    if (msg)
    {
        puts(msg);
    }
    exit(EXIT_FAILURE);
}

list l_open(long int data_size)
{
    list lst;

    lst = (list)malloc(LIST_SIZE);
    if (!lst)
    {
        l_error("L_OPEN: Memory allocation failed");
    }
    lst->size = data_size;
    lst->count = 0;
    lst->top = lst->bottom = NULL;

    return lst;
}

void l_close(list lst)
{
    if (!lst)
    {
        l_error("L_CLOSE: Memory allocation failed");
    }
    while (!l_empty(lst))
    {
        l_del_top(lst);
    }
    free(lst);
}

l_node l_create_node(long int size)
{
    l_node tmp;

    tmp = (l_node)malloc(L_NODE_SIZE);
    if (!tmp)
    {
        l_error("L_CREATE_NODE: Memory allocation failed");
    }
    if (size != L_POINTER)
    {
        tmp->data = malloc(size);
        if (!tmp->data)
        {
            l_error("L_CREATE_NODE: Memory allocation failed");
        }
    }
    else
    {
        tmp->data = NULL;
    }
    tmp->next = NULL;
    return tmp;
}

int l_empty(list lst)
{
    if (!lst)
    {
        l_error("L_EMPTY: no list is given");
    }
    if (!lst->top)
    {
        return 1;
    }
    return 0;
}

void *l_top(list lst)
{
    if (!lst)
    {
        l_error("L_TOP: no list is given");
    }
    if (l_empty(lst))
    {
        l_error("L_TOP: the list is empty");
    }
    return lst->top->data;
}

void *l_bot(list lst)
{
    if (!lst)
    {
        l_error("L_BOTTOM: no list is given");
    }
    if (l_empty(lst))
    {
        l_error("L_BOTTOM: the list is empty");
    }
    return lst->bottom->data;
}

void *l_at(list lst, int i)
{
    l_node tmp;

    if (!lst)
    {
        l_error("L_AT: no list is given");
    }
    if ((i < 0) || (i >= lst->count))
    {
        l_error("L_AT: out of bounds");
    }
    if (l_empty(lst))
    {
        l_error("L_DEL_AT: the list is empty!\n");
    }
    tmp = lst->top;
    while (i)
    {
        i--;
        tmp = tmp->next;
    }
    return tmp->data;
}

void l_add_top(list lst, void *data)
{
    if (!lst)
    {
        l_error("L_ADD_TOP: no list is given\n");
    }
    if (!data)
    {
        l_error("L_ADD_TOP: no data is given");
    }
    l_node tmp = l_create_node(lst->size);
    if (lst->size == L_POINTER)
    {
        tmp->data = data;
    }
    else
    {
        memcpy(tmp->data, data, lst->size);
    }
    tmp->next = lst->top;
    if (lst->top && !lst->bottom)
    {
        lst->bottom = lst->top;
        lst->top = tmp;
    }
    else
    {
        lst->top = tmp;
    }
    lst->count++;
}

void l_add_bot(list lst, void *data)
{
    l_node tmp;

    if (!lst)
    {
        l_error("L_ADD_BOTTOM: no list is given\n");
    }
    if (!data)
    {
        l_error("L_ADD_BOTTOM: no data is given");
    }
    tmp = l_create_node(lst->size);
    if (lst->size == L_POINTER)
    {
        tmp->data = data;
    }
    else
    {
        memcpy(tmp->data, data, lst->size);
    }
    if (!lst->top)
    {
        lst->top = tmp;
    }
    else if (!lst->bottom)
    {
        lst->top->next = tmp;
        lst->bottom = tmp;
    }
    else
    {
        lst->bottom->next = tmp;
        lst->bottom = tmp;
    }
    lst->count++;
}

// remove element from list.
void l_del_top(list lst)
{
    if (!lst)
    {
        l_error("L_ADD_TOP: no list is given\n");
    }
    if (l_empty(lst))
    {
        l_error("L_ADD_TOP: the list is empty!\n");
    }
    l_node tmp = lst->top;
    lst->top = lst->top->next;
    lst->count--;
    if (lst->size != L_POINTER)
    {
        free(tmp->data);
    }
    free(tmp);
}

// destroy list.
void l_del_bot(list lst)
{
    l_node tmp, tmp2;

    if (!lst)
    {
        l_error("L_DEL_BOTTOM: no list is given\n");
    }
    if (l_empty(lst))
    {
        l_error("L_DEL_BOTTOM: the list is empty!\n");
    }
    if (!lst->bottom)
    {
        tmp = lst->top;
        lst->top = NULL;
    }
    else
    {
        tmp2 = lst->top;
        while (tmp2->next != lst->bottom)
        {
            tmp2 = tmp2->next;
        }
        if (tmp2 == lst->top)
        {
            tmp = lst->bottom;
            lst->bottom = NULL;
            lst->top->next = NULL;
        }
        else
        {
            tmp = lst->bottom;
            lst->bottom = tmp2;
            lst->bottom->next = NULL;
        }
    }
    lst->count--;
    if (lst->size != L_POINTER)
    {
        free(tmp->data);
    }
    free(tmp);
}

void l_del_at(list lst, int i)
{
    l_node tmp, tmp2;

    if (!lst)
    {
        l_error("L_DEL_AT: no list is given");
    }
    if ((i < 0) || (i >= lst->count))
    {
        l_error("L_DEL_AT: out of bounds");
    }
    if (l_empty(lst))
    {
        l_error("L_DEL_AT: the list is empty!\n");
    }
    tmp = lst->top;
    tmp2 = lst->top;
    while (i)
    {
        i--;
        tmp2 = tmp;
        tmp = tmp->next;
    }
    if (tmp == lst->top)
    {
        if (tmp->next == lst->bottom)
        {
            lst->bottom = NULL;
        }
        lst->top = lst->top->next;
    }
    else if (tmp == lst->bottom)
    {
        tmp2->next = NULL;
        if (tmp->next == lst->bottom)
        {
            lst->bottom = NULL;
        }
        else
        {
            lst->bottom = tmp2;
        }
    }
    else
    {
        tmp2->next = tmp->next;
    }
    lst->count--;
    if (lst->size != L_POINTER)
    {
        free(tmp->data);
    }
    free(tmp);
}

// void print_list(list lst)
// {
//     if (!lst)
//     {
//         l_error("PRINT_LIST: the list is empty!\n");
//     }
//     if (lst)
//     {
//         l_node tmp = lst->top;
//         printf("%d: ", lst->count);
//         while (tmp)
//         {
//             printf("%d -> ", *(int *)(tmp->data));
//             tmp = tmp->next;
//         }
//         printf("NULL\n");
//     }
// }
// int main()
// {
//     list lst;
//     setbuf(stdout, NULL);
//     lst = l_open(sizeof(int));
//     for (int i = 0; i < 2; i++)
//     {
//         l_add_top(lst, &i);
//     }
//     for (int i = 3; i < 5; i++)
//     {
//         l_add_bottom(lst, &i);
//     }
//     printf("Nb: %d\n", lst->count);
//     print_list(lst);

//     l_del_at(lst, 2);
//     print_list(lst);

//     l_del_at(lst, 1);
//     print_list(lst);

//     l_del_at(lst, 0);
//     print_list(lst);

//     while (!l_empty(lst))
//     {
//         l_del_bottom(lst);
//         print_list(lst);
//     }
//     l_close(lst);
// }
