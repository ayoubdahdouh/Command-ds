#ifndef COLOR_H
#define COLOR_H

/**
 * standar files type
 **/
// rs (reset to no color)
// di (directory)
// ln (symbolic link)
// mh (multi-hardlink)
// pi (named pipe, AKA FIFO)
// so (socket)
// do (door)
// bd (block device)
// cd (character device)
// or (orphan symlink)
// su (set-user-ID)
// sg (set-group-ID)
// ca (file with capability)
// tw (sticky and other-writable directory)
// ow (other-writable directory)
// st (sticky directory)
// ex (executable file)

#include <stdbool.h>
#include "list.h"

typedef struct lfcolor
{
    char *a;
    char *c;
    bool e;
} lfcolor;

#define LFCOLORSIZ sizeof(lfcolor)

char *getcolor(LIST l, const char *nm, bool is_ext);
LIST scan_for_color();

#endif