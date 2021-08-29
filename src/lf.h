#ifndef LF_H
#define LF_H

#include "list.h"
#include "format_tree.h"
#include "common.h"

void run(linklist arguments);
void run();
void core(format_tree_t *tree);
int stringcmp(char *s1, char *s2);
void sort_tb(char **tb, int n);

void sort(linklist l);
int sort_name(lf_type t1, lf_type t2);
int sort_i(lf_type t1, lf_type t2);
int sort_n(lf_type t1, lf_type t2);
int sort_u(lf_type t1, lf_type t2);
int sort_g(lf_type t1, lf_type t2);
int sort_s(lf_type t1, lf_type t2);
int sort_a(lf_type t1, lf_type t2);
int sort_m(lf_type t1, lf_type t2);
int sort_c(lf_type t1, lf_type t2);
int sort_t(lf_type t1, lf_type t2);
int sort_e(lf_type t1, lf_type t2);

// #define sort_name(t1, t2) stringcmp((t1->name), (t2->name));

// #define sort_i(t1, t2) (t1->st.st_ino < t2->st.st_ino) ? -1 : 1;

// #define sort_n(t1, t2) (t1->st.st_nlink < t2->st.st_nlink) ? -1 : 1;

// #define sort_u(t1, t2) (t1->st.st_uid < t2->st.st_uid) ? -1 : 1;

// #define sort_g(t1, t2) (t1->st.st_gid < t2->st.st_gid) ? -1 : 1;

// #define sort_s(t1, t2) (t1->st.st_size < t2->st.st_size) ? -1 : 1;

// #define sort_a(t1, t2) (t1->st.st_atime < t2->st.st_atime) ? -1 : 1;

// #define sort_m(t1, t2) (t1->st.st_mtime < t2->st.st_mtime) ? -1 : 1;

// #define sort_c(t1, t2) (t1->st.st_ctime < t2->st.st_ctime) ? -1 : 1;

// #define sort_t(t1, t2) ((t1->st.st_mode) < (t2->st.st_mode)) ? -1 : 1;

#endif