#ifndef TYPES_H
#define TYPES_H

#include <sys/stat.h>

#define B0 (1 << 0)   // 0x1
#define B1 (1 << 1)   // 0x2
#define B2 (1 << 2)   // 0x4
#define B3 (1 << 3)   // 0x8
#define B4 (1 << 4)   // 0x10
#define B5 (1 << 5)   // 0x20
#define B6 (1 << 6)   // 0x40
#define B7 (1 << 7)   // 0x80
#define B8 (1 << 8)   // 0x100
#define B9 (1 << 9)   // 0x200
#define B10 (1 << 10) // 0x400
#define B11 (1 << 11) // 0x800
#define B12 (1 << 12) // 0x1000
#define B13 (1 << 13) // 0x2000
#define B14 (1 << 14) // 0x4000
#define B15 (1 << 15) // 0x8000
#define B16 (1 << 16) // 0x10000
#define B17 (1 << 17) // 0x20000
#define B18 (1 << 18) // 0x40000
#define B19 (1 << 19) // 0x80000

typedef enum
{
    False,
    True
} Bool;

// Options M parameters
#define MH B0
#define MD B1
#define MR B2
#define ML B3
#define MB B4
#define MC B5
#define MP B6
#define MS B7
#define MU B8
#define MG B9
#define MT B10
#define M1 B11
#define M2 B12
#define M3 B13
#define M4 B14
#define M5 B15
#define M6 B16
#define M7 B17
#define M8 B18
#define M9 B19

// Options L parameters
#define LI B0
#define LN B1
#define LO B2
#define LG B3
#define LP B4
#define LS B5
#define LA B6
#define LM B7
#define LC B8
#define LR B9

// Options N parameters
#define NC B0
#define NB B1
#define NF B2
#define NQ B3
#define NI B4

// Options S parameters
#define SI B0
#define SN B1
#define SS B2
#define SU B3
#define SG B4
#define SA B5
#define SM B6
#define SC B7
#define ST B8
#define SE B9

// Options
#define O1 B0
#define O2 B1
#define O3 B2
#define O4 B3
#define OT B4
#define OL B5
#define OD B6
#define OM B7
#define OS B8
#define ON B9
#define OV B10
#define OH B11
#define OC B12
#define OF B13
#define OR B14
#define OI B14

typedef struct File
{
    char *name;
    struct stat st;
    int err;

} * File;
#define FILE_SIZE sizeof(struct File)

#define I_INDEX 0
#define N_INDEX 1
#define U_INDEX 2
#define G_INDEX 3
#define S_INDEX 4
#define P_INDEX 5
#define A_INDEX 6
#define M_INDEX 7
#define C_INDEX 8
typedef struct FileInfo
{
    char *bfr[9];
} FileInfo;

#define FILEINFO_SIZE sizeof(struct FileInfo)

typedef struct TreeInfo
{
    int level;
    char *has_next;
} TreeInfo;
#define TREEINFO_SIZE sizeof(TreeInfo)
#define TREEMAXDEPTH (PATH_MAX - 1)

typedef struct Color
{
    char *a;
    char *c;
    Bool e;
} Color;

#define COLOR_SIZE sizeof(Color)

#endif