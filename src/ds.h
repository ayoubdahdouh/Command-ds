#ifndef LF_H
#define LF_H

#include "linkedList.h"
#include "useTree.h"
#include "common.h"

Bool checkMode(mode_t *mode);
void core(TreeInfo *tree);

void Sort(LinkedList l);
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