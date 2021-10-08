#ifndef ARGS_H
#define ARGS_H

#include "types.h"
#include "linkedList.h"

void setTParam(char **c);
void setSParam(char **c);
void setMParams(char **c);
void setLParams(char **c);
void setNParams(char **c);
void setFParam(char **c);
void setOptions(int argc, char *argv[], linkedList l);

#endif