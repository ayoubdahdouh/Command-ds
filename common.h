#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct lf_options
{
    int a;       // all, by default lf doesn't show hidden file.
    int c;       // for a colorful output.
    int d;       // show only directories.
    int f;       // show only files.
    int g;       // show groups of files.
    int l;       // show file's informations(permissions and size).
    int m;       // show modification time of each file.
    int p;       // show permissions of files.
    int h;       // used with -s, make the size readable like 4K, 13M, 2G, etc
    int s;       // show sizes of files.
    int t;       // recursive(tree), by default lf shows current folder.
    int u;       // show the owners of files.
    int version; // version
    int help;
} lf_options;

typedef struct lf_t
{
    char *name;
    struct stat s;

} lf_t;

#define LF_OPTIONS_Z sizeof(struct lf_options)
#define LF_T_Z sizeof(struct lf_t)

#define RST "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"

#define ERR_SYSTEM 7
#define ERR_INVALID_OPTION 4
#define ERR_MEM_ALLOC 3
#define ERR_ACCESS_DENIED 2

extern lf_options opt;
extern char *path;
extern int pathsiz;
extern char *buf;
extern int with_colors;

void help();
void version();
int has_space(const char *name);
int is_absolute_path(const char *path);
void lf_error(int err, char *cause);
void *lf_malloc(long int size);
int lf_stat(const char *fname, struct stat *s);
void lf_print_color(const char *fname, mode_t *mode);
void lf_print(char *fname, mode_t *mode); // afficher le nom des fichiers
void lf_path_error(int err);

#endif