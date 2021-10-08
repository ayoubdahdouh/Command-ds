#ifndef FORMAT_COLUMN
#define FORMAT_COLUMN 1

#include "list.h"
#include "types.h"

void column_main(linklist l, FileInfo *files_info, int index);
void column_display(linklist l, int *ls, int *lm, FileInfo *files_info, int index, int *ts, int *tm, int cl, int ln);

#endif