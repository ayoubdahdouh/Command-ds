#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "list.h"

// char *media[16] = {"aac", "au", "flac", "m4a", "mid",
//                    "midi", "mka", "mp3", "mpc", "ogg", "ra",
//                    "wav", "oga", "opus", "spx", "xspf"};
// char *image[49] = {"jpg", "jpeg", "mjpg", "mjpeg", "gif", "bmp",
//                    "pbm", "pgm", "ppm", "tga", "xbm", "xpm", "tif",
//                    "tiff", "png", "svg", "svgz", "mng", "pcx", "mov",
//                    "mpg", "mpeg", "m2v", "mkv", "webm", "ogm", "mp4",
//                    "m4v", "mp4v", "vob", "qt", "nuv", "wmv", "asf", "rm",
//                    "rmvb", "flc", "avi", "fli", "flv", "gl", "dl",
//                    "xcf", "xwd", "yuv", "cgm", "emf", "ogv", "ogx"};
// char *archive[46] = {"tar", "tgz", "arc", "arj", "taz", "lha", "lz4",
//                      "lzh", "lzma", "tlz", "txz", "tzo", "t7z", "zip", "z",
//                      "dz", "gz", "lrz", "lz", "lzo", "xz", "zst", "tzst",
//                      "bz2", "bz", "tbz", "tbz2", "tz", "deb", "rpm", "jar",
//                      "war", "ear", "sar", "rar", "alz", "ace", "zoo", "cpio",
//                      "7z", "rz", "ab", "wim", "swm", "dwm", "esd"};
// lfcolor colors[] = {
//     (lfcolor){.e = "rs", .c = NULL}, /* reset to no color */
//     (lfcolor){.e = "di", .c = NULL}, /* directory */
//     (lfcolor){.e = "ln", .c = NULL}, /* symbolic link */
//     (lfcolor){.e = "mh", .c = NULL}, /* multi-hardlink */
//     (lfcolor){.e = "pi", .c = NULL}, /* named pipe, AKA FIFO */
//     (lfcolor){.e = "so", .c = NULL}, /* socket */
//     (lfcolor){.e = "do", .c = NULL}, /* door */
//     (lfcolor){.e = "bd", .c = NULL}, /* block device */
//     (lfcolor){.e = "cd", .c = NULL}, /* character device */
//     (lfcolor){.e = "or", .c = NULL}, /* orphan symlink */
//     (lfcolor){.e = "mi", .c = NULL}, /* missing file */
//     (lfcolor){.e = "su", .c = NULL}, /* set-user-ID */
//     (lfcolor){.e = "sg", .c = NULL}, /* set-group-ID */
//     (lfcolor){.e = "ca", .c = NULL}, /* file with capability */
//     (lfcolor){.e = "tw", .c = NULL}, /* sticky and other-writable directory */
//     (lfcolor){.e = "ow", .c = NULL}, /* other-writable directory */
//     (lfcolor){.e = "st", .c = NULL}, /* sticky directory */
//     (lfcolor){.e = "ex", .c = NULL}, /* executable file */
//     (lfcolor){.e = "ar", .c = NULL}, /* archive */
//     (lfcolor){.e = "im", .c = NULL}, /* image */
//     (lfcolor){.e = "me", .c = NULL}  /* media */
// };

LIST scan_for_color()
{
    LIST l = LOPEN();
    lfcolor *c;
    char *b, *v;
    int n, i, eq = 0, sm = 0;

    v = getenv("LS_COLORS");
    if (v)
    {
        n = strlen(v);
        b = (char *)malloc(sizeof(char) * n);
        if (b)
        {
            strncpy(b, v, n);
        }
    }
    else
    {
        return NULL;
    }

    for (i = 0; i < n; i++)
    {
        if (v[i] == '=')
        {
            eq = i;
        }
        else if (v[i] == ':')
        {
            // (sm < eq - 1) := extension must be not empty
            // (eq < i - 1)) := color value must be not empty
            if (eq &&
                (sm < eq) &&
                (eq < i - 1))
            {
                v[eq] = 0;
                v[i] = 0;
                c = (lfcolor *)malloc(LFCOLORSIZ);
                c->c = &v[eq] + 1;
                c->a = &v[sm];
                // verify if "c->a" is an extension
                if (c->a[0] == '*' && c->a[1] == '.')
                {
                    c->a += 2;
                    c->e = 1;
                }
                else
                {
                    c->e = 0;
                }
                eq = 0;
                sm = i + 1;

                LADD(l, LFIRST, c);
            }
            else
            {
                // error
                sm = i + 1;
                eq = 0;
            }
        }
    }
    // case where there is no ":" at the of "b"
    if (i == n && eq)
    {
        v[eq] = 0;
        v[n - 1] = 0;
        c = (lfcolor *)malloc(LFCOLORSIZ);
        c->a = &v[sm];
        c->c = &v[eq] + 1;
        LADD(l, LFIRST, c);
    }
    return l;
}

char *getcolor(LIST l, const char *nm, int is_ext)
{
    int ok = 0;
    ITERATOR i;
    lfcolor *tmp;

    if (LEMPTY(l))
    {
        return NULL;
    }
    for (i = LAT(l, LFIRST); i && !ok; LINC(&i))
    {
        tmp = (lfcolor *)i->data;
        if ((is_ext == tmp->e) &&
            (strcmp(nm, tmp->a) == 0))
        {
            ok = 1;
        }
    }
    return tmp->c;
}