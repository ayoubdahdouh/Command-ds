#ifndef DISPLAY_H
#define DISPLAY_H

#include <sys/stat.h>
#include <stdbool.h>

void choose_color(char *nm, mode_t *m, char **c);
void display_name(char *nm, mode_t *m, char *c, char *r);
void display(char *nm, mode_t *m, bool nl);

#endif