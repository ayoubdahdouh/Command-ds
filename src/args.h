#ifndef ARGS_H
#define ARGS_H

#include "types.h"
#include "list.h"

int set_t_arg(char **c);
char set_s_arg(char **c);
_arg_m *set_m_arg(char **c);
_arg_l *set_i_arg(char **c);
_arg_n *set_n_arg(char **c);
char *set_f_arg(char **c);
void set_options(int argc, char *argv[], linklist l);

#endif