#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

int opt_index = 1;
char *opt_arg = NULL;

void usage() {
    printf("usage: mine [-r num] [-c num] [-p num]\n");
    printf("       Optionally specify field [-r] rows and [-c] cols (20-128)\n");
    printf("       Optionally specify [-p] percentage of mines (1-99) \n");
}

int options(int argc, char *argv[], const char *legal) {
    static char *posn = ""; /* position in argv[opt_index] */
    char *legal_index = NULL;
    int letter = 0;

    if (!*posn) {
        /* no more args, no SwitchChar or no option letter ? */
        if ((opt_index >= argc) ||
            (*(posn = argv[opt_index]) != switch_char) ||
            !*++posn)
            return -1;
        /* find double SwitchChar ? */
        if (*posn == switch_char) {
            opt_index++;
            return -1;
        }
    }
    letter = *posn++;
    if (!(legal_index = strchr(legal, letter))) {
        if (!*posn)
            opt_index++;
        return unknown_char;
    }
    if (*++legal_index != ':') {
        /* no option argument */
        opt_arg = NULL;
        if (!*posn)
            opt_index++;
    } else {
        if (*posn)
            /* no space between opt and opt arg */
            opt_arg = posn;
        else if (argc <= ++opt_index) {
            posn = "";
            return unknown_char;
        } else
            opt_arg = argv[opt_index];
        posn = "";
        opt_index++;
    }
    return letter;
}

void invalid_arg_exit() {
    usage();
    exit(EINVAL);
}