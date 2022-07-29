#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "mine.h"

int opt_index = 1;
char *opt_arg = NULL;

void usage() {
    printf("usage: mine [-r num] [-c num] [-p num]\n");
    printf("       Optionally specify field [-r] rows and [-c] cols (%zu-%zu)\n", FIELD_MIN_LIMIT, FIELD_MAX_LIMIT);
    printf("       Optionally specify [-p] percentage of mines (%zu-%zu) \n", PERCENTAGE_MIN_LIMIT, PERCENTAGE_MAX_LIMIT);
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
            opt_arg = NULL;
        } else
            opt_arg = argv[opt_index];
        posn = "";
        opt_index++;
    }
    return letter;
}

void get_arguments(int argc, char **argv) {
    int letter = -1;
    int value = -1;
    char *tail = "\0";

    if (argc > 1) {
        // Pre checking
        letter = options(argc, argv, "r:c:p:");
        value = opt_arg != NULL ? strtoul(opt_arg, &tail, 10) : -1;

        printf("%c, %d\n", letter, value);

        while (letter != -1) {
            if (letter == '?') {
                printf("invalid option: %s\n", argv[opt_index - 1]);
                invalid_arg_exit();
            } else if (value == -1 || tail[0] != '\0') {
                printf("invalid option argument: [-%c num] - must be followed by number\n", letter);
                invalid_arg_exit();
            }

            switch (letter) {
                case 'r':
                    if (value != 0) ROWS = value;
                    break;
                case 'c':
                    if (value != 0) COLS = value;
                    break;
                case 'p':
                    if (value != 0) PERCENTAGE = value;
                    break;
                default:
                    break;
            }

            tail = "\0";
            letter = options(argc, argv, "r:c:p:");
            value = opt_arg != NULL ? strtoul(opt_arg, &tail, 10) : -1;
        }
    }
}

void invalid_arg_exit() {
    usage();
    exit(EINVAL);
}