#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"
#include "mine.h"

int opt_index = 1;
char *opt_arg = NULL;
bool is_running;

/* Variable to store original terminal settings */
struct termios saved_tattr;

void usage() {
    printf("usage: mine [-r num] [-c num] [-p num]\n");
    printf("       Optionally specify field [-r] rows and [-c] cols (%u-%u)\n", FIELD_MIN_LIMIT, FIELD_MAX_LIMIT);
    printf("       Optionally specify [-p] percentage of mines (%u-%u) \n", PERCENTAGE_MIN_LIMIT, PERCENTAGE_MAX_LIMIT);
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

        while (letter != -1) {
            if (letter == '?') {
                printf("invalid option: %s\n", argv[opt_index - 1]);
                invalid_arg_exit();
            } else if (errno == ERANGE) {
                printf("invalid option argument: [-%c num] - number to large\n", letter);
                invalid_arg_exit();
            } else if (value == -1 || tail[0] != '\0' || errno == EINVAL) {
                printf("invalid option argument: [-%c num] - must be followed by number\n", letter);
                invalid_arg_exit();
            }

            switch (letter) {
                case 'r':
                    if (is_in_range(value, FIELD_MIN_LIMIT, FIELD_MAX_LIMIT))
                        ROWS = value;
                    else {
                        printf("for 'row' option [-%c]\n", letter);
                        invalid_arg_exit();
                    }
                    break;
                case 'c':
                    if (is_in_range(value, FIELD_MIN_LIMIT, FIELD_MAX_LIMIT))
                        COLS = value;
                    else {
                        printf("for 'col' option [-%c]\n", letter);
                        invalid_arg_exit();
                    }
                    break;
                case 'p':
                    if (is_in_range(value, PERCENTAGE_MIN_LIMIT, PERCENTAGE_MAX_LIMIT))
                        PERCENTAGE = value;
                    else {
                        printf("for 'percentage' option [-%c]\n", letter);
                        invalid_arg_exit();
                    }
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

bool is_in_range(int target, int min, int max) {
    if (target > max || target < min) {
        printf("provided argument: %d is not in range (%d-%d) ", target, min, max);
        return false;
    }
    return true;
}

void invalid_arg_exit() {
    usage();
    exit(EINVAL);
}

void reset_input_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_tattr);
}

void set_input_mode(void) {
    struct termios tattr;
    char *name;

    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "Not a terminal.\n");
        exit(EXIT_FAILURE);
    }

    tcgetattr(STDIN_FILENO, &saved_tattr);
    atexit(reset_input_mode);

    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO); /* Clear ICANON and ECHO.  */
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

void signal_handler(int signal) {
    switch (signal) {
        case SIGINT:
            is_running = false;
            break;

        default:
            break;
    }
}

bool yes() {
    char answer = '\0';

    do {
        scanf("%c", &answer);
        switch (answer) {
            case 'n':
            case 'N':
            case '\033':
                printf("\r\033[2K");
                return false;
                break;

            case 'y':
            case 'Y':
            case '\040':
            case '\012':
                printf("\r\033[2K");
                return true;
                break;

            default:
                break;
        }

    } while (1);
}

void clear_screen() {
    system("clear");
}
