#ifndef FORMAT_COLUMN
#define FORMAT_COLUMN 1

#include <stdbool.h>
#include "list.h"

void column_main(linklist l, char **tb);
void column_display(linklist l, int *ls, int *lm, char **tb, int *ts, int *tm, int cl, int ln);

#endif