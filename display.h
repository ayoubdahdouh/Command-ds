#ifndef DISPLAY_H
#define DISPLAY_H

#include <sys/stat.h>
#include <stdbool.h>

void lf_show_with_color(const char *nm, mode_t *m, bool nl);
void lf_show(char *nm, mode_t *m, bool nl);


#endif