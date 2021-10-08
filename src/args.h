#ifndef ARGS_H
#define ARGS_H

#include "types.h"
#include "list.h"

void set_t_arg(char **c);
void set_s_arg(char **c);
void set_m_arg(char **c);
void set_l_arg(char **c);
void set_n_arg(char **c);
void set_f_arg(char **c);
void set_options(int argc, char *argv[], linklist l);

#endif