#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "linkedList.h"
#include "common.h"

void setTParam(char **c)
{
    Bool ok = True;
    int cnt = 0;
    if (**c == ':')
    {
        ++*c;
        for (char *d = *c; ok && *d; ++d)
        {
            if (*d >= '0' && *d <= '9')
            {
                ++cnt;
            }
            else
            {
                ok = False;
                break;
            }
        }
        if (ok && cnt)
        {
            sscanf(*c, "%d", &Tparam);
            *c += cnt - 1;
        }
        else if (!ok)
        {
            quitProgram(PROGRAM ": the depth must be a positive number (without the + sign).");
        }
        else
        { // if (cnt == 0)
            quitProgram(PROGRAM ": 't' needs a parameter after ':'.");
        }
    }
    else
    {
        --*c;
        Tparam = TREEMAXDEPTH;
    }
}

void setSParam(char **c)
{
    if (**c == ':')
    {
        ++*c;
        if (**c)
        {
            if (strchr("dinsugamcte", **c))
            {
                if (*(*c + 1))
                {
                    quitProgram(PROGRAM ": The 's' accepts only one parameter.");
                }
                else
                {
                    switch (**c)
                    {
                    case 'd':
                        Sparams = SD;
                        break;
                    case 'i':
                        Sparams = SI;
                        break;
                    case 'n':
                        Sparams = SN;
                        break;
                    case 's':
                        Sparams = SS;
                        break;
                    case 'u':
                        Sparams = SU;
                        break;
                    case 'g':
                        Sparams = SG;
                        break;
                    case 'a':
                        Sparams = SA;
                        break;
                    case 'm':
                        Sparams = SM;
                        break;
                    case 'c':
                        Sparams = SC;
                        break;
                    case 't':
                        Sparams = ST;
                        break;
                    case 'e':
                        Sparams = SE;
                        break;
                    }
                }
            }
            else
            {
                printf("%s: 's' doesn't recognize '%c'.", PROGRAM, **c);
                quitProgram(NULL);
            }
        }
        else
        {
            quitProgram(PROGRAM ": The 's' needs a parameter after ':'.");
        }
    }
    else
    {
        --*c;
    }
}

void setMParams(char **c)
{
    Bool ok = True;
    char *d;

    if (**c == ':')
    {
        ++*c;
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'h':
                Mparams |= MH;
                break;
            case 'b':
                Mparams |= MB;
                break;
            case 'c':
                Mparams |= MC;
                break;
            case 'd':
                Mparams |= MD;
                break;
            case 'p':
                Mparams |= MP;
                break;
            case 'l':
                Mparams |= ML;
                break;
            case 'r':
                Mparams |= MR;
                break;
            case 's':
                Mparams |= MS;
                break;
            case 'u':
                Mparams |= MU;
                break;
            case 'g':
                Mparams |= MG;
                break;
            case 't':
                Mparams |= MT;
                break;
            case '1':
                Mparams |= M1;
                break;
            case '2':
                Mparams |= M2;
                break;
            case '3':
                Mparams |= M3;
                break;
            case '4':
                Mparams |= M4;
                break;
            case '5':
                Mparams |= M5;
                break;
            case '6':
                Mparams |= M6;
                break;
            case '7':
                Mparams |= M7;
                break;
            case '8':
                Mparams |= M8;
                break;
            case '9':
                Mparams |= M9;
                break;
            default:
                ok = False;
                --d;
                break;
            }
        }
        if (ok && d != *c)
        {
            *c = d - 1;
        }
        else if (!ok)
        {
            printf("%s: 'm' doesn't recognize '%c'.\n", PROGRAM, *d);
            quitProgram(NULL);
        }
        else
        { // if (*d==c)
            quitProgram(PROGRAM ": 'm' needs a parameter after ':'.");
        }
    }
    else
    {
        --*c;
    }
}

void setLParams(char **c)
{
    Bool ok = True;
    char *d;

    if (**c == ':')
    {
        ++*c;
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'i':
                Lparams |= LI;
                break;
            case 'n':
                Lparams |= LN;
                break;
            case 'p':
                Lparams |= LP;
                break;
            case 's':
                Lparams |= LS;
                break;
            case 'r':
                Lparams |= LR;
                break;
            case 'u':
                Lparams |= LU;
                break;
            case 'g':
                Lparams |= LG;
                break;
            case 'a':
                Lparams |= LA;
                break;
            case 'm':
                Lparams |= LM;
                break;
            case 'c':
                Lparams |= LC;
                break;
            default:
                ok = False;
                --d;
                break;
            }
        }
        if (ok && d != *c)
        {
            *c = d - 1;
        }
        else if (!ok)
        {
            printf("%s: 'l' doesn't recognize '%c'.\n", PROGRAM, *d);
            quitProgram(NULL);
        }
        else
        { // if (*d == c)
            quitProgram(PROGRAM ": The 'l' needs a parameter after ':'.\n");
        }
    }
}

void setNParams(char **c)
{
    Bool ok = True;
    char *d;

    if (**c == ':')
    {
        ++*c;
        for (d = *c; ok && *d; ++d)
        {
            switch (*d)
            {
            case 'c':
                Nparams |= NC;
                break;
            case 'b':
                Nparams |= NB;
                break;
            case 'f':
                Nparams |= NF;
                break;
            case 'q':
                Nparams |= NQ;
                break;
            case 'i':
                Nparams |= NI;
                break;
            default:
                ok = False;
                --d;
                break;
            }
        }
        if (ok && d != *c)
        {
            *c = d - 1;
        }
        else if (!ok)
        {
            printf("%s: 'n' doesn't recognize '%c'.\n", PROGRAM, *d);
            quitProgram(NULL);
        }
        else
        { //  if (d == *c)
            quitProgram(PROGRAM ": 'n' needs a parameter after ':'.");
        }
    }
    else
    {
        --*c;
    }
}

void setFParam(char **c)
{
    if ((**c == ':') && *(*c + 1))
    {
        ++*c;
        int cnt = strlen(*c);
        TimeStyle = (char *)memAlloc((cnt + 1));
        strncpy(TimeStyle, *c, cnt);
        *c += cnt - 1;
    }
    else
    {
        --*c;
        TimeStyle = NULL;
    }
}

void setOptions(int argc, char *argv[], linkedList l)
{
    Bool ok = True;

    char *c;
    memset(&Opts, 0, sizeof(Opts));
    memset(&Mparams, 0, sizeof(Mparams));
    memset(&Lparams, 0, sizeof(Lparams));
    memset(&Nparams, 0, sizeof(Nparams));
    memset(&Sparams, 0, sizeof(Sparams));
    --argc;
    while (ok && argc)
    {
        c = argv[argc];
        if (*c == '-')
        {
            ++c;
            for (; ok && *c; ++c)
            {
                switch (*c)
                {
                case '1':
                    Opts |= O1;
                    break;
                case '2':
                    Opts |= O2;
                    break;
                case '3':
                    Opts |= O3;
                    break;
                case '4':
                    Opts |= O4;
                    break;
                case 'c':
                    Opts |= OC;
                    break;
                case 'd':
                    Opts |= OD;
                    break;
                case 'v':
                    Opts |= OV;
                    break;
                case 'h':
                    Opts |= OH;
                    break;
                case 'r':
                    Opts |= OR;
                    break;
                case 't':
                    Opts |= OT;
                    ++c;
                    setTParam(&c);
                    break;
                case 's':
                    Opts |= OS;
                    ++c;
                    setSParam(&c);
                    break;
                case 'm':
                    Opts |= OM;
                    ++c;
                    setMParams(&c);
                    break;
                case 'l':
                    Opts |= OL;
                    ++c;
                    setLParams(&c);
                    if (!countActiveBits(Lparams, 10))
                    {
                        --c;
                    }
                    break;
                case 'n':
                    Opts |= ON;
                    ++c;
                    setNParams(&c);

                    break;
                case 'f':
                    Opts |= OF;
                    ++c;
                    if (TimeStyle)
                    {
                        free(TimeStyle);
                    }
                    setFParam(&c);
                    break;
                default:
                    --c;
                    ok = False;
                    break;
                }
            }
        }
        else
        {
            // it's a file/folder.
            lInsert(l, LFIRST, c);
        }
        --argc;
    }
    if (!ok)
    {
        printf("%s: Unknown option '%c'\n", PROGRAM, *c);
        quitProgram(NULL);
    }
}
