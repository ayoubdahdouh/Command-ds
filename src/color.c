#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "linkedList.h"
#include "common.h"
#include "types.h"

LinkedList scanForColors()
{
    LinkedList l = lOpen();
    Color *c;
    char *b, *v;
    int n, i, eq = 0, sm = 0;

    v = getenv("LS_COLORS");
    if (!v)
    {
        return l;
    }
    n = strlen(v);
    b = (char *)memAlloc(n);
    strncpy(b, v, n);
    for (i = 0; i < n; i++)
    {
        if (v[i] == '=')
        {
            eq = i;
        }
        else if (v[i] == ':')
        {
            // (sm < eq - 1) := extension must be not empty
            // (eq < i - 1)) := color value must be not empty
            if (eq &&
                (sm < eq) &&
                (eq < i - 1))
            {
                v[eq] = 0;
                v[i] = 0;
                c = (Color *)malloc(COLOR_SIZE);
                c->c = &v[eq] + 1;
                c->a = &v[sm];
                // verify if "c->a" is an extension
                if (c->a[0] == '*' && c->a[1] == '.')
                {
                    c->a += 2;
                    c->e = True;
                }
                else
                {
                    c->e = False;
                }
                eq = 0;
                sm = i + 1;

                lInsert(l, LFIRST, c);
            }
            else
            {
                // error
                sm = i + 1;
                eq = 0;
            }
        }
    }
    // case where there is no ":" at the of "b"
    if (i == n && eq)
    {
        v[eq] = 0;
        v[n - 1] = 0;
        c = (Color *)malloc(COLOR_SIZE);
        c->a = &v[sm];
        c->c = &v[eq] + 1;
        lInsert(l, LFIRST, c);
    }
    return l;
}

char *getColor(LinkedList l, const char *nm, Bool is_ext)
{
    int ok = 0;
    Iterator i;
    Color *tmp = NULL;

    if (lEmpty(l))
    {
        return NULL;
    }
    for (i = lAt(l, LFIRST); i && !ok; lInc(&i))
    {
        tmp = (Color *)i->data;
        if ((is_ext == tmp->e) &&
            (strcmp(nm, tmp->a) == 0))
        {
            ok = 1;
        }
    }
    if (ok)
    {
        return tmp->c;
    }
    else
    {
        if (strcmp(nm, "rs") == 0)
        {
            return NULL;
        }
        else
        {
            return getColor(l, "rs", False);
        }
    }
}