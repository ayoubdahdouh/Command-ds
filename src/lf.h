#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"
#include "common.h"

Bool validate_mode(mode_t *mode);
void core(TreeInfo *tree);
int strCmp(char *s1, char *s2);

void Sort(linklist l);
int sortNames(File t1, File t2);
int sortI(File t1, File t2);
int sortN(File t1, File t2);
int sortU(File t1, File t2);
int sortG(File t1, File t2);
int sortS(File t1, File t2);
int sortA(File t1, File t2);
int sortM(File t1, File t2);
int sortC(File t1, File t2);
int sortT(File t1, File t2);
int sortE(File t1, File t2);

#endif