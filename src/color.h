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

#include "linkedList.h"
#include "types.h"

char *getColor(LinkedList l, const char *nm, Bool is_ext);
LinkedList scanForColors();

#endif