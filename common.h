#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#define PROGRAM "lf"
#define VERSION "1.0.0-alpha+pre"

typedef struct lfoptions
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
} lfoptions;

typedef struct lftype
{
    char *name;
    struct stat st;

} *lftype;

#define OPTIONSIZ sizeof(struct lfoptions)
#define LFSIZ sizeof(struct lftype)

#define RST "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"

#define ERR_SYSTEM 7
#define ERR_INVALID_OPTION 4
#define ERR_MEM_ALLOC 3
#define ERR_ACCESS_DENIED 2

extern lfoptions opt;
extern char *path;
extern int pathsiz;
extern char *buf;
extern int with_colors;

void help();
void version();
int has_space(const char *name);
int is_absolute_path(const char *path);
void lf_error(int err, char *cause);
void *lfalloc(long int size);
int lf_stat(const char *fname, struct stat *s);
void lf_show_color(const char *fname, mode_t *mode, int nl);
void lf_show(char *fname, mode_t *mode, int nl); // afficher le nom des fichiers
void lf_path_error(int err);

#endif