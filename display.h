#ifndef DISPLAY_H
#define DISPLAY_H

#include <sys/stat.h>
#include <stdbool.h>

void lfprint_colored(char *nm, mode_t *m, bool nl, bool fl);
void lfprint(char *nm, mode_t *m, bool nl, bool fl);

#endif