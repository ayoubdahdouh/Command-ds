#ifndef DISPLAY_H
#define DISPLAY_H

#include <sys/stat.h>
#include "types.h"

void chooseColor(char *nm, mode_t *m, char **c);
void displayName(char *nm, mode_t *m, char *c, char *r);
void display(char *nm, mode_t *m, Bool nl);

#endif